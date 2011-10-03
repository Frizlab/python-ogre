# /* Original author: John C. Hare
#  * Date: June 26, 1996 (although I've had these around for at least 6 years)
#  * Adapted to C++ by W.J. van der Laan 2004
#  */

/**
 * Simple, fast, inline quaternion math functions
 */
class Quat:
      r=0
      i=0
      j=0
      k=0

def qadd(a, b)
        a.r += b.r
        a.i += b.i
        a.j += b.j
        a.k += b.k
 

def qmult(Quat &c, const Quat &a, const Quat &b) 
 
        c.r = a.r*b.r - a.i*b.i - a.j*b.j - a.k*b.k;
        c.i = a.r*b.i + a.i*b.r + a.j*b.k - a.k*b.j;
        c.j = a.r*b.j + a.j*b.r + a.k*b.i - a.i*b.k;
        c.k = a.r*b.k + a.k*b.r + a.i*b.j - a.j*b.i;
 

def qsqr(Quat &b, const Quat &a) 
 
        b.r = a.r*a.r - a.i*a.i - a.j*a.j - a.k*a.k;
        b.i = 2.0f*a.r*a.i;
        b.j = 2.0f*a.r*a.j;
        b.k = 2.0f*a.r*a.k;
 

/**
 * Implicit function that evaluates the Julia set.
 */
class Julia  
private:
	float global_real, global_imag, global_theta;
	Quat oc,c,eio,emio;
public:
	Julia(float global_real, float global_imag, float global_theta);
	inline float eval(float x, float y, float z)  
		Quat q, temp;
		int i;
	
		q.r = x;
		q.i = y;
		q.j = z;
		q.k = 0.0;
	
		for (i = 30; i > 0; i--)  
			qsqr(temp, q);
			qmult(q, emio, temp);
			qadd(q, c);
	
			if (q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k > 8.0)
				break;
		 
	
		return((float)i);
	 
 ;


Julia::Julia(float global_real, float global_imag, float global_theta):
		global_real(global_real), global_imag(global_imag), global_theta(global_theta)   

	oc.r = global_real;
	oc.i = global_imag;
	oc.j = oc.k = 0.0;

	eio.r = cos(global_theta);
	eio.i = sin(global_theta);
	eio.j = 0.0;
	eio.k = 0.0;

	emio.r = cos(-global_theta);
	emio.i = sin(-global_theta);
	emio.j = 0.0;
	emio.k = 0.0;

	/***
	 *** multiply eio*c only once at the beginning of iteration
	 *** (since q |-> sqrt(eio*(q-eio*c)))
	 *** q -> e-io*q^2 - eio*c
	 ***/

	qmult(c, eio,oc);

 


#endif
