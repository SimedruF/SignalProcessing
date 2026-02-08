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
        Group group = file->createGroup("/" + groupName);
        StrType strType(PredType::C_S1, H5T_VARIABLE);
        DataSpace scalarSpace(H5S_SCALAR);
        DataSet dataset = group.createDataSet(key, strType, scalarSpace);
        dataset.write(value, strType);
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

private:
    H5File* file = nullptr;

    // Creează recursiv grupuri de forma "/Sensors/Vibrations"
    Group createGroupsRecursively(const std::string& path) {
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
