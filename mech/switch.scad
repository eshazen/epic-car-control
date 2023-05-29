//
// OS102011MA1QN1 switch
//
$fn = 32;

bod_x = 8.6;
bod_y = 4.4;
bod_z = 3.9;

sw_x = 2.0;
sw_y = 4.0;
sw_z = 2.0;

pin_d = 0.8;
pin_g = 1.2;
pin_z = 2.8;

module pin_at_dia( px, py, pd) {
     translate( [px, py, -pin_z])
	  color("gold")
	  cylinder( d=pd, h=pin_z);
}

module body() {
     color("lightgrey")
     translate( [-bod_x/2, -bod_y/2, 0])
	  cube( [bod_x, bod_y, bod_z]);
}

module sw() {
     translate( [1, -bod_y/2-sw_y ,bod_z/2-sw_z/2])
	  color("black")
	  cube( [sw_x, sw_y, sw_z]);
}

module switch() {
     body();
     sw();
     pin_at_dia( -2, 0, pin_d);
     pin_at_dia( 0, 0, pin_d);
     pin_at_dia( 2, 0, pin_d);
     pin_at_dia( -4.1, 0, pin_g);
     pin_at_dia( 4.1, 0, pin_g);
}

switch();


