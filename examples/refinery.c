# include "refinery.h"
main()
{
	int	p_alarm,c_alarm,alarm;
	sensor_rec	pump_sensor,flow_sensor;
	sensor_rec	presure_sensor,level_sensor;

	while (1){

		get_sensor_v(&pump_sensor);
		get_sensor_f(&flow_sensor);
		get_sensor_p(&presure_sensor);
		get_sensor_l(&level_sensor);

		presure_sys(&presure_sensor,&level_sensor);
		coolant_sys(&pump_sensor,&flow_sensor);
		p_alarm = !(presure_ok && level_ok);
		c_alarm = !(pump_ok && flow_ok);
		alarm = c_alarm || p_alarm;
		if (alarm) system_shutdown();
	}
}
