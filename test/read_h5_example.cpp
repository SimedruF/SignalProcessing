/**
 * Exemplu de citire a datelor din fișiere HDF5 create cu SignalRecorder
 * 
 * Compilare (Windows):
 * g++ -std=c++11 read_h5_example.cpp -o read_h5 -I"C:/HDF5/include" -L"C:/HDF5/lib" -lhdf5_cpp -lhdf5
 * 
 * Compilare (Linux):
 * g++ -std=c++11 read_h5_example.cpp -o read_h5 -lhdf5_cpp -lhdf5
 */

#include "H5Cpp.h"
#include <iostream>
#include <vector>
#include <string>

using namespace H5;

/**
 * Citește un vector de float dintr-un dataset HDF5
 */
std::vector<float> readFloatVector(const std::string& filename, 
                                   const std::string& datasetPath) 
{
    std::vector<float> data;
    
    try {
        // Deschide fișierul în mod read-only
        H5File file(filename, H5F_ACC_RDONLY);
        
        // Deschide dataset-ul
        DataSet dataset = file.openDataSet(datasetPath);
        
        // Obține informații despre dimensiuni
        DataSpace dataspace = dataset.getSpace();
        int rank = dataspace.getSimpleExtentNdims();
        
        if (rank != 1) {
            std::cerr << "Error: Expected 1D dataset, got " << rank << "D" << std::endl;
            return data;
        }
        
        hsize_t dims[1];
        dataspace.getSimpleExtentDims(dims, NULL);
        
        std::cout << "Dataset size: " << dims[0] << " elements" << std::endl;
        
        // Redimensionează vectorul
        data.resize(dims[0]);
        
        // Citește datele
        dataset.read(data.data(), PredType::NATIVE_FLOAT);
        
        // Citește atributul "units" dacă există
        if (dataset.attrExists("units")) {
            Attribute attr = dataset.openAttribute("units");
            StrType strType = attr.getStrType();
            std::string units;
            attr.read(strType, units);
            std::cout << "Units: " << units << std::endl;
        }
        
        file.close();
        
    } catch (FileIException &error) {
        error.printErrorStack();
    } catch (DataSetIException &error) {
        error.printErrorStack();
    } catch (DataSpaceIException &error) {
        error.printErrorStack();
    }
    
    return data;
}

/**
 * Citește metadata (string) dintr-un grup
 */
std::string readMetadata(const std::string& filename,
                        const std::string& groupName,
                        const std::string& key)
{
    std::string value;
    
    try {
        H5File file(filename, H5F_ACC_RDONLY);
        
        // Deschide grupul
        Group group = file.openGroup("/" + groupName);
        
        // Deschide dataset-ul metadata
        DataSet dataset = group.openDataSet(key);
        
        // Citește string-ul
        StrType strType = dataset.getStrType();
        dataset.read(value, strType);
        
        file.close();
        
    } catch (Exception &error) {
        error.printErrorStack();
    }
    
    return value;
}

/**
 * Listează toate dataset-urile dintr-un fișier HDF5
 */
void listAllDatasets(const std::string& filename) {
    try {
        H5File file(filename, H5F_ACC_RDONLY);
        
        std::cout << "\n=== Structure of " << filename << " ===" << std::endl;
        
        // Funcție callback pentru iterare
        auto printInfo = [](hid_t loc_id, const char *name, const H5L_info_t *info, void *opdata) -> herr_t {
            H5O_info_t obj_info;
            H5Oget_info_by_name(loc_id, name, &obj_info, H5P_DEFAULT);
            
            std::string indent = "  ";
            
            if (obj_info.type == H5O_TYPE_GROUP) {
                std::cout << indent << "[GROUP] " << name << std::endl;
            } else if (obj_info.type == H5O_TYPE_DATASET) {
                std::cout << indent << "[DATASET] " << name << std::endl;
            }
            
            return 0;
        };
        
        // Iterează prin toate obiectele
        hsize_t idx = 0;
        H5Literate(file.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, &idx, 
                   [](hid_t loc_id, const char *name, const H5L_info_t *info, void *opdata) -> herr_t {
                       std::cout << "  /" << name << std::endl;
                       return 0;
                   }, NULL);
        
        file.close();
        
    } catch (Exception &error) {
        error.printErrorStack();
    }
}

/**
 * Exemplu de utilizare
 */
int main(int argc, char** argv) {
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename.h5>" << std::endl;
        std::cout << "\nExemplu de utilizare:" << std::endl;
        std::cout << "  " << argv[0] << " sensor_data.h5" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    
    std::cout << "=== HDF5 File Reader ===" << std::endl;
    std::cout << "Reading: " << filename << std::endl;
    
    // 1. Listează structura fișierului
    listAllDatasets(filename);
    
    // 2. Exemplu: Citește metadata
    std::cout << "\n=== Reading Metadata ===" << std::endl;
    std::string deviceName = readMetadata(filename, "Metadata", "device_name");
    if (!deviceName.empty()) {
        std::cout << "Device Name: " << deviceName << std::endl;
    }
    
    // 3. Exemplu: Citește date de senzor
    std::cout << "\n=== Reading Sensor Data ===" << std::endl;
    
    // Presupunem că ai date salvate la: /Sensors/Vibrations/accelerometer_x
    std::vector<float> accelX = readFloatVector(filename, "/Sensors/Vibrations/accelerometer_x");
    
    if (!accelX.empty()) {
        std::cout << "\nAccelerometer X data:" << std::endl;
        std::cout << "  Total samples: " << accelX.size() << std::endl;
        std::cout << "  First 10 values: ";
        for (size_t i = 0; i < std::min(accelX.size(), size_t(10)); i++) {
            std::cout << accelX[i] << " ";
        }
        std::cout << std::endl;
        
        // Calcul statistici simple
        float sum = 0, min = accelX[0], max = accelX[0];
        for (float val : accelX) {
            sum += val;
            if (val < min) min = val;
            if (val > max) max = val;
        }
        float avg = sum / accelX.size();
        
        std::cout << "  Min: " << min << std::endl;
        std::cout << "  Max: " << max << std::endl;
        std::cout << "  Average: " << avg << std::endl;
    }
    
    return 0;
}
