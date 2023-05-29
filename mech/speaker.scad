
bod_d = 12;
bod_z = 8.5;

pin_d = 0.6;
pin_z = 5;

$fn=32;

module pin_at(x,y) {
     translate( [x, y, -pin_z])
	  cylinder( h=pin_z, d=pin_d);
}

module speaker() {
     difference() {
	  color("darkgrey")
	       cylinder( d=bod_d, h=bod_z);
	  translate( [0, 0, bod_z-1])
	       cylinder( d=3, h=1.5);
     }
     pin_at( -3.25, 0);
     pin_at( 3.25, 0);
}

speaker();

