#ifndef POWERTASK_STORAGE_H
#define POWERTASK_STORAGE_H

/**
 * @brief Save in storage
 * 
 * @param[in] data_to_store Data to be stored
 * @param[in] size_of_data  Size of data to be stored
 * 
 * @return 0, if successful
 * @return negative value, otherwise
*/
int powertask_storage_save(void *data_to_store, size_t size_of_data);

/**
 * @brief Load data from storage
 * 
 * @param[out] buffer Buffer in which loaded data will be copied to
 * @param[in] size_of_data  Size of the buffer
 * 
 * @return 0, if successful
 * @return negative value, otherwise
*/
int powertask_storage_load(void *buffer, size_t size_of_buffer);

#endif /* POWERTASK_STORAGE_H */
