
$fn = 64;
e = 0.1;
mm = 25.4;

show_wheels = 0;
show_brackets = 0;
show_chassis = 0;
show_both_sides = 0;
short_chassis = 0;
show_crossbar = 1;

pivot_hole = 0.14*mm;

// angle to steer
// steer_angle = ($t-0.5)*20;
steer_angle = 0;

// update to actual wheels
wheel_dia = 2.0*mm;
wheel_wid = 1*mm;
vehicle_wid = 4.0*mm;

wheelbase = 6.142*mm;

axle_dia = 0.35*mm;
axle_len = wheel_wid + 0.5*mm;

wheel_pivot_offset = 0.5*mm;

pivot_spc = vehicle_wid-2*wheel_wid-2*wheel_pivot_offset;
angle = atan2(wheelbase,pivot_spc/2);

bracket_stock_wid = 0.25*mm;
bracket_arm_len = 0.75*mm;
bracket_stock_thk = 0.125*mm;

// reference point where differential would be
//color("red") cylinder( h=30, d=1);

chassis_len = 7.5*mm;
chassis_thk = 0.125*mm;
chassis_tab_wid = 1.375*mm;
chassis_tab_len = 1.75*mm;

w = vehicle_wid;
l = chassis_len;
b = chassis_tab_len;
d = chassis_tab_wid;
a = (vehicle_wid-chassis_tab_wid)/2;
c = l-2*b;
pts_s = [ [0, b+c], [a, b+c], [a, l], [a+d, l], [a+d, l-b],
	  [0, b+c ]];
pts_l = [ [a, 0], [a, b], [0, b], [0, b+c], [a, b+c], [a, l],
	  [a+d, l], [a+d, l-b], [w, l-b], [w, b], [w-a, b],
	  [w-a, 0], [a, 0]];

module chassis() {
     if( short_chassis) 
	  linear_extrude( height=chassis_thk) polygon( points = pts_s);
     else
	  linear_extrude( height=chassis_thk) polygon( points = pts_l);
}

module crossbar() {
  // calculate endpoints of steering bar
  // left wheel
  left_x = wheel_pivot_offset-(vehicle_wid/2-wheel_wid) +
     cos(angle-steer_angle) * bracket_arm_len;
  left_y = wheelbase - sin(angle) * bracket_arm_len;
  left_z = 0;

  // right wheel
  right_x = -(wheel_pivot_offset-(vehicle_wid/2-wheel_wid) +
	      cos(angle+steer_angle) * bracket_arm_len);
  right_y = wheelbase - sin(angle) * bracket_arm_len;
  right_z = 0;

  crossbar_len = right_x - left_x;


  // draw the crossbar
  difference() {
       translate( [left_x, left_y, left_z+bracket_stock_thk]) {
	    // rounded ends
	    cylinder( d=bracket_stock_wid, h=bracket_stock_thk);
	    translate( [crossbar_len, 0, 0])
		 cylinder( d=bracket_stock_wid, h=bracket_stock_thk);
	    translate( [0, -bracket_stock_wid/2, 0])
		 cube( [crossbar_len, bracket_stock_wid, bracket_stock_thk]);
       }
       // indicate the pivot points
       translate( [left_x, left_y, -10]) cylinder( d=pivot_hole, h=20);
       translate( [right_x, right_y, -10]) cylinder( d=pivot_hole, h=20);
  }
       
}

// create a steering bracket
module bracket() {

     translate( [-wheel_pivot_offset, -bracket_stock_wid/2, 0])
	  cube( [wheel_pivot_offset, bracket_stock_wid, bracket_stock_thk]);

     rotate( [0, 0, -angle]) {
       translate( [0, -bracket_stock_wid/2, 0]) {
	    difference() {
		 union() {
		      cube( [bracket_arm_len, bracket_stock_wid, bracket_stock_thk]);
		      translate( [bracket_arm_len, bracket_stock_wid/2, 0])
			   cylinder( h=bracket_stock_thk, d=bracket_stock_wid);
		 }
		 translate( [bracket_arm_len, bracket_stock_wid/2, -1])
		      cylinder( d=pivot_hole, h=10);
	    }
       }
     }
     cylinder( h=bracket_stock_thk, d=bracket_stock_wid);
}


module wheel_at( x, y, z, steer) {
  translate( [ x+wheel_pivot_offset, y, z]) {
    rotate( [0, 0, steer]) {
      translate( [-wheel_pivot_offset, 0, 0]) {
	rotate( [0, 270, 0]) {
	     if( show_wheels) {
	     % cylinder( d=wheel_dia, h=wheel_wid);
		  translate( [0, 0, -(axle_len-wheel_wid)/2])
		       color("green") cylinder( d=axle_dia, h=axle_len);
	     }
	}
	// bracket with steering pivot
	if( show_brackets)
	translate( [wheel_pivot_offset, 0, -bracket_stock_thk/2]) {
	     difference() {
		  bracket();
		  translate( [0, 0, -1])
		       cylinder( d=pivot_hole, h=10);
	     }
	}
      }
    }
  }
}

// left front wheel
wheel_at( -(vehicle_wid/2-wheel_wid), wheelbase, 0, steer_angle);
// right front wheel
if( show_both_sides)
     mirror( [1, 0 ,0]) wheel_at( -(vehicle_wid/2-wheel_wid), wheelbase, 0, -steer_angle);

if( show_crossbar) crossbar();
if( show_chassis) {
     difference() {
	  translate( [-vehicle_wid/2, -chassis_tab_len/2, -5]) color("green") chassis();
	  translate( [-(vehicle_wid/2-wheel_wid)+wheel_pivot_offset, wheelbase, -10]) cylinder( d=pivot_hole, h=20);
	  mirror( [1, 0 ,0]) translate( [wheel_pivot_offset-(vehicle_wid/2-wheel_wid), wheelbase, -10]) cylinder( d=pivot_hole, h=20);
     }
}

