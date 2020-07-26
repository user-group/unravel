# include "refinery.h"
int coolant_sys(p,f)
	sensor_ptr	p,f;
{

	volt = p->value;
	flow = f->value;
	pump_ok = pump_undervolt(volt);
	flow_ok = coolant_flow(flow);
}
