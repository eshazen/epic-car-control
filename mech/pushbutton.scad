$fn = 32;

bod_x = 6.2;
bod_y = 6.2;
bod_z = 3.5;

plu_d = 3.5;
plu_z = 0.8;

plt_t = 0.2;
plt_r = 0.5;

pin_x = 0.6;
pin_y = 0.3;
pin_z = 5.5;

module body() {
     translate( [-bod_x/2, -bod_y/2, 0])
	  color("black")
	  cube( [bod_x, bod_y, bod_z-plt_t]);
}

cornerz = [ for ( x = [-bod_x/2+plt_r, bod_x/2-plt_r]) 
	   for( y = [-bod_x/2+plt_r, bod_x/2-plt_r]) [x, y] ];

module plate() {
     translate( [0, 0, bod_z-plt_t])
     color("lightgray")
     union() {
	  for( p = cornerz) {
	       translate( [ p[0], p[1], 0])
	       cylinder( d=2*plt_r, h=plt_t);
	  }
	  translate( [-bod_x/2+plt_r, -bod_y/2, 0])
	       cube( [bod_x-2*plt_r, bod_y, plt_t]);
	  translate( [-bod_x/2, -bod_y/2+plt_r, 0])
	       cube( [bod_x, bod_y-2*plt_r, plt_t]);
     }
     translate( [0, 0, bod_z]) {
	  for( p = cornerz) {
	       translate( [ p[0], p[1], 0])	       
		    color("black")
		    cylinder( d=0.8, h=0.2);
	  }
     }
}


module pin_at(x,y) {
     translate( [x-pin_x/2, y-pin_y/2, -pin_z])
	  cube( [pin_x, pin_y, pin_z]);
}

module plunger() {
     translate( [0, 0, bod_z])
	  color("black")
	  cylinder( d=3.5, h=0.25);
}

module switch() {
     body();
     plate();
     plunger();
     pin_at( -2.5, 0);
     pin_at( 2.5, 0);
}


switch();
