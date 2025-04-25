
$fn = 32;
e = 0.1;
mm = 25.4;

// update to actual wheels
wheel_dia = 2.0*mm;
wheel_wid = 1*mm;
vehicle_wid = 4.0*mm;

wheelbase = 6.142*mm;

axle_dia = 0.25*mm;
axle_len = wheel_wid + 0.5*mm;

wheel_pivot_offset = 0.5*mm;

pivot_spc = vehicle_wid-2*wheel_wid-2*wheel_pivot_offset;
angle = atan2(wheelbase,pivot_spc/2);

bracket_stock_wid = 0.25*mm;
bracket_arm_len = 0.75*mm;
bracket_stock_thk = 0.125*mm;

// reference point where differential would be
color("red")
cylinder( h=30, d=1);

chassis_len = 7.5*mm;
chassis_thk = 0.125*mm;
chassis_tab_wid = 1.375*mm;
chassis_tab_len = 1.75*mm;

module chassis() {
  w = vehicle_wid;
  l = chassis_len;
  b = chassis_tab_len;
  d = chassis_tab_wid;
  a = (vehicle_wid-chassis_tab_wid)/2;
  c = l-2*b;
  linear_extrude( height=chassis_thk)
  polygon( points =[ [a, 0], [a, b], [0, b], [0, b+c], [a, b+c], [a, l],
		     [a+d, l], [a+d, l-b], [w, l-b], [w, b], [w-a, b],
		     [w-a, 0], [a, 0]]);
}

module crossbar() {
  // calculate endpoints of steering bar
  // left wheel
  left_x = wheel_pivot_offset-(vehicle_wid/2-wheel_wid) +
     cos(angle) * bracket_arm_len;
  left_y = wheelbase - sin(angle) * bracket_arm_len;
  left_z = 0;

  // right wheel
  right_x = -(wheel_pivot_offset-(vehicle_wid/2-wheel_wid) +
	      cos(angle) * bracket_arm_len);
  right_y = wheelbase - sin(angle) * bracket_arm_len;
  right_z = 0;

  crossbar_len = right_x - left_x;

  // indicate the pivot points
  // translate( [left_x, left_y, left_z]) color("orange") cylinder( h=50, d=2);
  // translate( [right_x, right_y, right_z]) color("orange") cylinder( h=50, d=2);

  // draw the crossbar
  color("cyan")
  translate( [left_x, left_y, left_z+bracket_stock_thk]) {
    // rounded ends
    cylinder( d=bracket_stock_wid, h=bracket_stock_thk);
    translate( [crossbar_len, 0, 0])
      cylinder( d=bracket_stock_wid, h=bracket_stock_thk);
    translate( [0, -bracket_stock_wid/2, 0])
      cube( [crossbar_len, bracket_stock_wid, bracket_stock_thk]);
  }
}

// create a steering bracket
// center about pivot hole
module bracket() {

     translate( [-wheel_pivot_offset, -bracket_stock_wid/2, 0])
	  cube( [wheel_pivot_offset, bracket_stock_wid, bracket_stock_thk]);

     rotate( [0, 0, -angle]) {
       translate( [0, -bracket_stock_wid/2, 0]) {
	 cube( [bracket_arm_len, bracket_stock_wid, bracket_stock_thk]);
	 translate( [bracket_arm_len, bracket_stock_wid/2, 0]) {
	   // now we're at tie rod attach point
	   cylinder( h=bracket_stock_thk, d=bracket_stock_wid);
	   color("violet") cylinder( h=25, d=1);
	 }
       }
     }
     cylinder( h=bracket_stock_thk, d=bracket_stock_wid);
}


module wheel_at( x, y, z) {

  translate( [ x, y, z]) {
    rotate( [0, 270, 0]) {
      % cylinder( d=wheel_dia, h=wheel_wid);
      translate( [0, 0, -(axle_len-wheel_wid)/2])
	color("green") cylinder( d=axle_dia, h=axle_len);
    }
    // steering pivot
    translate( [wheel_pivot_offset, 0, 0]) {
      color("blue") cylinder( h=25, d=1);
      bracket();
    }
  }
}

// left front wheel
wheel_at( -(vehicle_wid/2-wheel_wid), wheelbase, 0);
// right front wheel
mirror( [1, 0 ,0]) wheel_at( -(vehicle_wid/2-wheel_wid), wheelbase, 0);

crossbar();
translate( [-vehicle_wid/2, -chassis_tab_len/2, -5])
color("green")
chassis();
