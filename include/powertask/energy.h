#ifndef POWERTASK_ENERGY_H
#define POWERTASK_ENERGY_H

/** @brief Energy source */
typedef struct powertask_energy_source_s {
    int capacitance; /**< Capacitance (in uF) of the energy source. */
    int (*get_voltage)(void); /**< Function to measure voltage (in mV) on the energy source. */
} powertask_energy_source_t; 

/** 
 * @brief Gets current available amount of energy  
 * 
 * @param[in] energy_source Energy source structure
 * 
 * @retval If positive, the amount of available energy in joules.
 * @retval -EINVAL \p energy_source is NULL or contains invalid parameter values.
 */
int powertask_get_available_energy(powertask_energy_source_t *energy_source);

#endif /* POWERTASK_ENERGY_H */
