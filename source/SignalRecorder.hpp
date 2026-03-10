#pragma once
#include "H5Cpp.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

using namespace H5;

class SignalRecorder {
public:
    SignalRecorder(const std::string& filename) {
        try {
            file = new H5File(filename, H5F_ACC_TRUNC);
        } catch (FileIException &e) {
            std::cerr << "Error creating HDF5 file: " << e.getDetailMsg() << std::endl;
        }
    }

    ~SignalRecorder() {
        if (file) {
            file->close();
            delete file;
        }
    }

    void addMetadata(const std::string& groupName, const std::string& key, const std::string& value) {
        H5::Exception::dontPrint();
        try {
            Group group;
            try {
                group = file->openGroup("/" + groupName);
            } catch (...) {
                group = file->createGroup("/" + groupName);
            }
            
            StrType strType(PredType::C_S1, H5T_VARIABLE);
            DataSpace scalarSpace(H5S_SCALAR);
            DataSet dataset = group.createDataSet(key, strType, scalarSpace);
            dataset.write(value, strType);
        } catch (Exception &e) {
            std::cerr << "Error adding metadata: " << e.getDetailMsg() << std::endl;
        }
    }

    void addFloatVector(const std::string& path,
                        const std::string& datasetName,
                        const std::vector<float>& data,
                        const std::string& units = "") 
    {
        hsize_t dims[1] = { data.size() };
        DataSpace dataspace(1, dims);

        Group group = createGroupsRecursively(path);

        DataSet dataset = group.createDataSet(
            datasetName,
            PredType::NATIVE_FLOAT,
            dataspace
        );

        dataset.write(data.data(), PredType::NATIVE_FLOAT);

        if (!units.empty()) {
            StrType strType(PredType::C_S1, H5T_VARIABLE);
            DataSpace attrSpace(H5S_SCALAR);
            Attribute attr = dataset.createAttribute("units", strType, attrSpace);
            attr.write(strType, units);
        }
    }
    
    // Add double vector (for timestamps with high precision)
    void addDoubleVector(const std::string& path,
                         const std::string& datasetName,
                         const std::vector<double>& data,
                         const std::string& units = "") 
    {
        hsize_t dims[1] = { data.size() };
        DataSpace dataspace(1, dims);

        Group group = createGroupsRecursively(path);

        DataSet dataset = group.createDataSet(
            datasetName,
            PredType::NATIVE_DOUBLE,
            dataspace
        );

        dataset.write(data.data(), PredType::NATIVE_DOUBLE);

        if (!units.empty()) {
            StrType strType(PredType::C_S1, H5T_VARIABLE);
            DataSpace attrSpace(H5S_SCALAR);
            Attribute attr = dataset.createAttribute("units", strType, attrSpace);
            attr.write(strType, units);
        }
    }
    
    // Add synchronized multi-channel data with timestamps
    void addSynchronizedChannels(const std::string& path,
                                 const std::vector<double>& timestamps,
                                 const std::vector<std::pair<std::string, std::vector<float>>>& channels)
    {
        // Save timestamps
        addDoubleVector(path, "timestamps", timestamps, "seconds");
        
        // Save each channel
        for (const auto& channel : channels) {
            addFloatVector(path, channel.first, channel.second);
        }
        
        // Add metadata about synchronization
        char sync_info[256];
        sprintf(sync_info, "%d", (int)channels.size());
        addMetadata(path, "num_channels", sync_info);
        sprintf(sync_info, "%d", (int)timestamps.size());
        addMetadata(path, "num_samples", sync_info);
    }

private:
    H5File* file = nullptr;

    // Creează recursiv grupuri de forma "/Sensors/Vibrations"
    Group createGroupsRecursively(const std::string& path) {
        // Disable HDF5 error messages for cleaner output
        H5::Exception::dontPrint();
        
        std::string currentPath;
        Group group = file->openGroup("/");

        size_t start = 0, pos;
        while ((pos = path.find('/', start)) != std::string::npos) {
            std::string part = path.substr(start, pos - start);
            if (!part.empty()) {
                currentPath += "/" + part;
                try {
                    group = file->openGroup(currentPath);
                } catch (...) {
                    group = file->createGroup(currentPath);
                }
            }
            start = pos + 1;
        }

        std::string last = path.substr(start);
        if (!last.empty()) {
            currentPath += "/" + last;
            try {
                group = file->openGroup(currentPath);
            } catch (...) {
                group = file->createGroup(currentPath);
            }
        }

        return group;
    }
};
