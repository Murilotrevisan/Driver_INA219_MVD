/*
 * INA219_MVD.c
 *
 *  Created on: Sep 8, 2021
 *      Author: Murilo Henrique Pasini Trevisan
 */

#include "INA219_MVD.h"


error_t INA219_reset(INA219_t ina) {

		uint16_t config_rst = 0x0000;
		config |= INA_RST;

		return i2c_write16(ina.device, INA_CONFIG_ADR, config_rst);
}

error_t INA219_config(INA219_t ina) {

		uint16_t config_register = 0x0000;

		config_register |= ina.config.BusVoltageRange;
		config_register |= ina.config.ShuntVoltageRange;
		config_register |= ina.config.BusADCResolution;
		config_register |= ina.config.ShuntADCResolution;

		return i2c_write16(ina.device, INA_CONFIG_ADR, config_register);
}

error_t INA219_measure(INA219_t ina, INA219_values_t *medida) {

		//Shunt Voltage
		//Leitura do valor i2c no registrador do shunt
		result16_t raw_s = i2c_read16(ina.device, INA_SHUNT_VOLT);

		//Analise de erro
		if (raw_s.hasError) {
			return 1;
		}
		//conversao do valor lido para tensão no Shunt
		medida -> Shunt_Voltage = ((float) raw_s.value) * INA_Shunt_Multiply; //Configurar valores

		//Bus Voltage
		//Leitura do valor i2c do registrador Bus
		result16_t raw_b = i2c_read16(ina.device, INA_BUS_VOLT);

		//Analise de erro
		if (raw.hasError) {
			return 1;
		}

		uint16_t Val_Shift = raw_b.value >> 3;
		//conversão do valor lida para tensão no Bus
		medida -> Bus_Voltage = ((float) Val_Shift) * INA_Bus_Multiply; //configurar valores

		//Current
		//Calculo a partir da tensão no Shunt e do valor do resistor do datasheet
		result16_t raw_c = (Shunt_Voltage)/(INA_Resistor_Shunt);

		medida -> Current_Shunt = ((float) raw_c); //configurar valores


}

