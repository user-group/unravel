/*
***********************************************************
*                                                         *
*  Ray tracing example  (unfinished)                      *
*                                                         *
***********************************************************
*/
/*
	# include <stdio.h>
	# include <math.h>
*/

typedef struct sphere_struct {
	float	x,y,z,r;
	} sphere;
typedef struct point_struct {
	float	x,y,z;
	} point;
typedef struct poly_struct {
	int	n_sides;
	int	points[30];
	} polygon;

int	n_spheres,n_points,n_poly;
sphere	s[20];
point	p[2000];
polygon g[200];
char	sx[60][60];

int get_pixel_size ()
{
	int	n;

	scanf ("%d",&n);
	return n;
}

void get_spheres()
{
	int		i,n;
	float	x,y,z,r;

	scanf ("%d",&n);
	n_spheres = n;
	for (i = 0; i < n_spheres; i++){
		scanf ("%f%f%f%f",&x,&y,&z,&r);
		s[i].r = r;
		s[i].x = x;
		s[i].y = y;
		s[i].z = z;
	}
}
void get_poly()
{
	int		i,ix,j,np,ng,ns;
	float	x,y,z,s;

	scanf ("%d",&np);
	n_points = np;
	i = 0;
	while (i < n_points){
		scanf ("%d %f %f %f",&ix,&x,&y,&z);
		p[ix].x = x;
		p[ix].y = y;
		p[ix].z = z;
		i++;
	}
	scanf ("%d",&n_poly);
	for (i = 0; i < n_poly; i++){
		scanf ("%d",&ns);
		g[i].n_sides = ns;
		for (j = 0; j < ns; j++)
			scanf ("%d",&g[i].points[j]);
	}
}

int	intersect_sphere (i,mx,bx,my,by,mz,bz,mint)
	float mx,bx,my,by,mz,bz;
	float	*mint;
{
	float	t,a,b,c,d,t0,t1;

	a = mx*mx + my*my + mz*mz;
	b = 2.0*( mx*(bx - s[i].x) + my*(by - s[i].y) + mz*(bz - s[i].z));
	c = bx*bx + by*by + bz*bz +
		s[i].x*s[i].x + s[i].y*s[i].y + s[i].z*s[i].z -
		2.0*(s[i].x*bx + s[i].y*by + s[i].z*bz) - 
		s[i].r*s[i].r;
	d = b*b - 4.0*a*c;
	/*
	printf ("a(%.2f) b(%.2f) c(%.2f) d(%.2f)",a,b,c,d);
	if (d < 0.0) printf ("\n");
	*/
	if (d < 0.0) return 0;
	t0 = (-b + sqrt(d))/(2.0*a);
	t1 = (-b - sqrt(d))/(2.0*a);
	if (t0 < t1) *mint = t1;
	else *mint = t0;
	/* printf (" t0(%.2f) t1(%.2f) mint(%.2f)\n",t0,t1,*mint); */
	return 1;
}

int	find_sphere (mx,bx,my,by,mz,bz)
	float mx,bx,my,by,mz,bz;
{
	int		i,ix = -1;
	float	minz,mins;

	if (n_spheres <= 0) return -1;
	for (i = 0; i < n_spheres; i++){
		if (intersect_sphere (i,mx,bx,my,by,mz,bz,&minz)){
			ix = i;
			break;
		}
	}
	for (i = ix + 1; i < n_spheres; i++){
		if (intersect_sphere (i,mx,bx,my,by,mz,bz,&mins)){
			if (mins > minz){ /* really want max (-z is away) */
				ix = i;
				minz = mins;
			}
		}
	}
	return ix;
}

float trace_ray (mx,bx,my,by,mz,bz)
	float mx,bx,my,by,mz,bz;
{
	int		sphere_index;
	float	t,x,y,z;

	sphere_index = find_sphere (mx,bx,my,by,mz,bz);
	if (sphere_index < 0) return 0.0;
	else return 1.0;
}

float	cast (i,j,n)
	int		i,j,n;
{
	float	u,v,shade;

	/* create a ray from Center of Projection (0,0,0) */
	/* through the screen pixel (x,y)				  */
	/* assume screen upper left origin				  */
	/* assume screen parallel to xy plane at z = -1   */
	/* limits of view plane are -1 < x,y < 1	  	  */
	/*       Pixel to view plane					  */
	/*       (i,j) -> (u,v)							  */
	/*       (0,0) -> (-1,1)						  */
	/*       (n-1,n-1) -> (1,-1)					  */
	/*       U(I) = (2I)/(n-1) - 1					  */
	/*       V(J) = -(2J)/(n-1) + 1					  */

	u =  ((float) 2*i)/((float) (n-1)) - 1.0;
	v = -((float) 2*j)/((float) (n-1)) + 1.0;
	/* printf ("i,j (%d,%d) (%.2f,%.2f) ",i,j,u,v); */
	shade = trace_ray (u,0.0,v,0.0,-1.0,0.0);
	return shade;
}

main ()
{
	int		n_pixels;
	float	screen[300][300];
	int		x,y;

	for (x = 0; x < 60; x++)
		for (y = 0; y < 60; y++) sx[x][y] = ' ';
	n_pixels = get_pixel_size ();
	get_spheres();
	get_poly();
	if (n_pixels > 300) n_pixels = 300;
	for (x = 0; x < n_pixels; x++)
		for (y = 0; y < n_pixels; y++)
			screen[x][y] = cast (x,y,n_pixels);

	printf ("Screen is %dx%d pixels\n",n_pixels,n_pixels);
	printf ("There are %d spheres\n",n_spheres);
	printf ("There are %d vertices\n",n_points);
	for (x = 0; x < 60; x++)
		for (y = 0; y < 60; y++)
			if (screen[x][y] > 0.0) sx[x][y] = '*';
	for (y = 0; y < 60; y++){
		for (x = 0; x < 60; x++)
			printf ("%c ",sx[x][y]);
		printf ("\n");
	}
}
