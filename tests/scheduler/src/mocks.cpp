#include <CppUTestExt/MockSupport.h>

#include <string.h>
#include <errno.h>

#define MAX_FAKE_STORAGE_LEN 1024

extern "C" {
    #include <powertask/energy.h>

    static uint8_t fake_storage[MAX_FAKE_STORAGE_LEN];
    static size_t fake_storage_used;

    int powertask_get_available_energy(powertask_energy_source_t *energy_source){
        return mock().actualCall("powertask_get_available_energy").returnIntValue();
    }

    int powertask_storage_save(void *data_to_store, size_t size_of_data){

        if (data_to_store == NULL || size_of_data == 0) {
            return -EINVAL;
        }

        memcpy(fake_storage, data_to_store, size_of_data);
        fake_storage_used = size_of_data;

        return mock().actualCall("powertask_storage_save").returnIntValue();
    }

    int powertask_storage_load(void *buffer, size_t size_of_buffer){

        if (fake_storage == NULL || fake_storage_used == 0){
            return -EINVAL;
        }

        memcpy(buffer, fake_storage, fake_storage_used);

        return mock().actualCall("powertask_storage_load").returnIntValue();
    }

    void fake_clear_powertask_storage(void){
        memset(fake_storage, 0, MAX_FAKE_STORAGE_LEN);
        fake_storage_used = 0;
    }
}
