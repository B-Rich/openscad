// Value vector tests.

a1=[0,1,2];
b1=[3,4,5];
c1=a1*b1;
echo(str("Testing vector dot product: ",c1));

d1=[1,0];
echo(str("  Bounds check: ",a1*d1));

m2=[[0,1],[1,0]];
v2=[2,3];
p2=m2*v2;
echo(str("Testing matrix * vector: ",p2));

d2=[0,0,1];
echo(str("  Bounds check: ",m2*d2));

m3=[[1,-1,1],[1,0,-1]];
v3=[1,1];
p3=v3*m3;
echo(str("Testing vector * matrix: ",p3));

echo(str("  Bounds check: ",m3*v3));

ma4=[ [1,0],[0,1] ];
mb4=[ [1,0],[0,1] ];
echo(str("Testing id matrix * id matrix: ",ma4*mb4));

ma5=[ [1, 0, 1]
     ,[0, 1,-1] ];
mb5=[ [1,0]
     ,[0,1]
     ,[1,1] ];
echo(str("Testing asymmetric matrix * matrix: ",ma5*mb5));
echo(str("Testing alternate asymmetric matrix * matrix: ",mb5*ma5));

echo(str("  Bounds check: ",ma5*ma4));

m6=ma5*mb5;

echo(str("Testing matrix+vectorMatrix addition: ",m6+m2));
echo(str("Testing vectorMatrix+matrix addition: ",m2+m6));
echo(str("Testing matrix+matrix addition: ",m6+m6));

echo(str("Testing matrix-vectorMatrix subtraction: ",m6-m2));
echo(str("Testing vectorMatrix-matrix subtraction: ",m2-m6));
echo(str("Testing matrix-matrix subtraction: ",m6-m6));

echo(str("Testing scalar*matrix: ",4*m6));
echo(str("Testing matrix*scalar: ",m6*4));

echo(str("Texting matrix/scalar: ",m6/4));
echo(str("Testing vectorMatrix/scalar: ",[[2,1],[-1,0]]/4));
echo(str("Testing scalar/matrix, i.e. scalar * matrix.inverse(): ",1/m6));
echo(str("Testing scalar/vectorMatrix: ",1/[[2,1],[-1,0]]));
echo(str("Testing matrix * (1/matrix): ", m6 * (1/m6) ) );
degenerate_m1=identity(2)*[[1,0],[0,0]];
echo(str("  Bounds check 1/[[1,0],[0,0]]: ",1/degenerate_m1));

echo(str("Matrix row access: ",m6[0]));

cube(1.0);
