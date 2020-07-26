# include "refinery.h"
int presure_sys(p,w)
	sensor_ptr	p,w;
{

	presure = p->value;
	level = w->value;
	presure_ok = check_presure(presure);
	level_ok = water_level(level);
}
