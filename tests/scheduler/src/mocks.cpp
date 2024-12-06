#include <CppUTestExt/MockSupport.h>

extern "C" {
    int powertask_get_available_energy(){
        return mock().actualCall("powertask_get_available_energy").returnIntValue();
    }
    int powertask_storage_save(void *data_to_store, size_t size_of_data){
        return mock().actualCall("powertask_storage_save").returnIntValue();
    }

    int powertask_storage_load(void *buffer, size_t size_of_buffer){

        return mock().actualCall("powertask_storage_load").returnIntValue();
    }
}
