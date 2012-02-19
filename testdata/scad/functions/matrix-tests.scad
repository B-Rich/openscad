m1=[[2,0],[0,3]];
echo("Original Matrix: ",m1);
m2=pow(m1,0);
echo("Identity from original matrix: ",m2);
for(i=[0:10]) echo(str("Power ",i,":",pow(m1,i)));
echo(str("identity(2)=",identity(2)));
echo(str("identity(10)=",identity(10)));
echo("Bounds check:",pow(m1,-1));
echo("Bounds check:",identity(-2));
echo("Asymmetric matrix pow bounds check, longer rows:",pow([[1,2,3],[4,5,6]],1));
echo("Asymmetric matrix pow bounds check, shorter rows:",pow([[1,2],[3,4],[5,6]],1));

cube();
