$fa = 1;
$fs = 0.5;

module bearing () {
    translate([0, 0, 10.5-4])
    sphere(d = 8);
    cylinder(h = 8.7, d = 12);
}

module bearing_holder() {
    rotate([0, 50, 0])
    difference() {
        translate([0, 0, -32])
        rotate([0, -50, 0])
        translate([0, 0, -7])
        cylinder(h = 11, d = 17);
            
        translate([-5, -10, -26-5])
        cube([20, 20, 20]);
    }
}

difference() {
    union() {
        translate([0, 0, 28.25]) {
            rotate([0, 0, 0])
            bearing_holder();

            rotate([0, 0, 120])
            bearing_holder();

            rotate([0, 0, 240])
            bearing_holder();
        }

        difference() {
            cylinder(h = 3, d = 66.5);
             
            translate([-21+8, -13/2, -1])
            cube([21, 13, 5]);
             
            translate([0, 0, -1])
            cylinder(h = 2.1, d = 18);
            translate([0, 0, 1])
            cylinder(h = 3, d1 = 18, d2 = 28);
            
            translate([0, 25, -1])
            cylinder(h = 7, d = 3);
            
            translate([0, -25, -1])
            cylinder(h = 7, d = 3);
            
            
            rotate([0, 0, 230])
            translate([55, 0, -1])
            cylinder(h = 7, d = 70);
            
            rotate([0, 0, 130])
            translate([55, 0, -1])
            cylinder(h = 7, d = 70);
            
            translate([45, 0, -1])
            cylinder(h = 7, d = 60);
        }
    }

    translate([0, 0, 28.25]) {
        rotate([0, 50, 0])
        translate([0, 0, -26])
        translate([0, 0, -10.5])
        bearing();
        rotate([0, 50, 120])
        translate([0, 0, -26])
        translate([0, 0, -10.5])
        bearing();
        rotate([0, 50, 240])
        translate([0, 0, -26])
        translate([0, 0, -10.5])
        bearing();
    }
}

if (true
    ) {
    translate([0, 0, 28.25]) {
        sphere(d = 52.5);

        rotate([0, 50, 0])
        translate([0, 0, -26])
        translate([0, 0, -10.5])
        bearing();
        rotate([0, 50, 120])
        translate([0, 0, -26])
        translate([0, 0, -10.5])
        bearing();
        rotate([0, 50, 240])
        translate([0, 0, -26])
        translate([0, 0, -10.5])
        bearing();
    }
}

