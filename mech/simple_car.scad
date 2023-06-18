$fn=64;

chassis_z = 0.125;

module chassis() {
     color("gray")
     linear_extrude( height=chassis_z, center=true, convexity=10) {
	  import("modified_chassis.dxf", layer="0");
     }
}

module wheel() {
     color("black")
	  cylinder( h=1, d=2);
}     

wheel_z = -0.4;

module wheels() {
     rotate( [90, 0, 0]) {
	  translate( [3, wheel_z, 1]) wheel();
	  translate( [3, wheel_z, -2]) wheel();
	  translate( [-3, wheel_z, 1]) wheel();
	  translate( [-3, wheel_z, -2]) wheel();
     }
}

mot_x = 2.5;
mot_y = 0.75;
mot_z = 1;

chassis_x = 7.4;

module motor() {
     translate( [(-chassis_x/2)+0.55, -mot_y/2, chassis_z])
	  color("yellow")
	  cube([mot_x, mot_y, mot_z]);
}

batt_x = 2.25;
batt_y = 2.50;
batt_z = 0.625;

module battery() {
     translate( [(-chassis_x/2)+0.55+mot_x, -batt_y/2, chassis_z])
	  color("#908020")
	  cube( [batt_x, batt_y, batt_z]);
}


module opto() {
     scale( [0.0394, 0.0394, 0.0394])
	  color("#800080")
	  import("TCRT5000.stl");
}


pcb_above_batt = 0.125;

module arduino() {
	  // imported arduino from STL
	  color("cyan")
	       translate( [2, -0.35, 0.2])
	       rotate( [0, 0, 90])
	       scale( [0.0394, 0.0394, 0.0394])
	       import("Pro_Micro.stl");
	  //
}

module pcb() {
     translate( [0, 0, chassis_z+batt_z+pcb_above_batt]) {
	  // PCB outline from DXF
	  color("red")
	       linear_extrude( height=0.063, center=true, convexity=10) {
	       import("modified_chassis.dxf", layer="PCB");
	  }
//	  rotate( [270, 0, 0]) translate( [3, 0, 1.4]) opto();
     }
}


chassis();
wheels();
motor();
battery();
pcb();
