
// example010.dat generated using octave:
//   d = (sin(1:0.2:10)' * cos(1:0.2:10)) * 10;
//   save("example010.dat", "d");
render_part=2;

if(render_part==1) {
intersection()
{
	surface(file = "example010.dat",
		center = true, convexity = 5);
	
	rotate(45, [0, 0, 1])
	surface(file = "example010.dat",
		center = true, convexity = 5);
}
}

if(render_part==2) {
  scale([0.1,0.1,1]) surface(file="example010.png");
}
