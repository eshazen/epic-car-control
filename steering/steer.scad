
$fn = 32;
e = 0.1;
mm = 25.4;

// update to actual wheels
wheel_dia = 2.0*mm;
wheel_wid = 0.75*mm;
vehicle_wid = 4.0*mm;

wheelbase = 6.142*mm;

axle_dia = 0.25*mm;
axle_len = wheel_wid + 0.5*mm;

wheel_pivot_offset = 0.5*mm;

pivot_spc = vehicle_wid-2*wheel_wid-2*wheel_pivot_offset;
angle = atan2(wheelbase,pivot_spc/2);

// reference point where differential would be
color("red")
cylinder( h=30, d=1);

// create a steering bracket
// center about pivot hole
module bracket() {
     bracket_stock_wid = 0.25*mm;
     bracket_arm_len = 1*mm;
     bracket_stock_thk = 0.125*mm;

     translate( [-wheel_pivot_offset, -bracket_stock_wid/2, 0])
	  cube( [wheel_pivot_offset, bracket_stock_wid, bracket_stock_thk]);

     rotate( [0, 0, -angle]) {
       translate( [0, -bracket_stock_wid/2, 0]) {
	 cube( [bracket_arm_len, bracket_stock_wid, bracket_stock_thk]);
	 translate( [bracket_arm_len, bracket_stock_wid/2, 0]) {
	   // now we're at tie rod attach point
	   cylinder( h=bracket_stock_thk, d=bracket_stock_wid);
	   color("violet") cylinder( h=50, d=1);
	 }
       }
     }
     cylinder( h=bracket_stock_thk, d=bracket_stock_wid);
}

// left front wheel
translate( [ -(vehicle_wid/2-wheel_wid), wheelbase, 0]) {
  rotate( [0, 270, 0]) {
       % cylinder( d=wheel_dia, h=wheel_wid);
       translate( [0, 0, -(axle_len-wheel_wid)/2])
       color("green") cylinder( d=axle_dia, h=axle_len);
  }
  // steering pivot
  translate( [wheel_pivot_offset, 0, 0]) {
       color("blue") cylinder( h=50, d=1);
       bracket();
  }
}

