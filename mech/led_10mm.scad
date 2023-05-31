//
// generic 10mm LED
//

$fn = 64;

dia = 10.0;
rad = 5;
hgt = 11.6-rad;
base_z = 2.0;
base_dia = 11;

pin_d = 0.5;
pin_z = 25;
pin_s = 2.54;

// bend pin at this Z
pin_b = 2;

module pin_at( x, y, ra, pl) {
  if( ra == 1) {
    // right-angle
    translate( [x, y, -pin_b]) {
      cylinder( d=pin_d, h=pin_b);
      rotate( [90, 0, 0])
	cylinder( d=pin_d, h=pl-pin_b);
    }
  } else {
    // vertical
    translate( [x, y, -pl])
      cylinder( d=pin_d, h=pin_z);
  }
}

module led( colr, ra) {
  color("gold") {
    pin_at( -2.54/2, 0, ra, pin_z+1);
    pin_at( 2.54/2, 0, ra, pin_z);
  }
  color( colr) {
    cylinder( h=base_z, d=base_dia);
    translate( [0, 0, base_z])
      cylinder( d=dia, h=hgt);
    translate( [0, 0, base_z+hgt])
      sphere( d=dia);
  }
}

// white
// led( [1, 1, 1, 0.75], 1);
// red
led( [1, 0, 0, 0.75], 1);

  
