#include <ivfmath/IvfVec3d.h>
#include <ivfmath/IvfVec4d.h>
#include <ivfmath/IvfMat3d.h>
#include <ivfmath/IvfMat4d.h>

int main(int argc, char* argv[])
{
	using namespace std;

	CIvfVec3d a(1.0, 0.0, 0.0);
	CIvfVec3d b(0.0, 1.0, 0.0);

	CIvfVec3d c;
	c = a + b;
	
	CIvfVec3d d;
	d = a * b;
	
	CIvfVec3d e;
	e = d;

	cout << "Vectors c, d, e." << endl << endl;

	c.print(cout);
	d.print(cout);
	e.print(cout);

	CIvfVec3d axis(0.0, 1.0, 0.0);
	CIvfVec3d rotated(1.0, 0.0, 0.0);

	rotated.rotate(axis, 45.0);

	cout << endl << "Rotated matrix rotated." << endl << endl;
	rotated.print(cout);

	CIvfMat3d A;
	A = 0.0;

	cout << endl << "Matrix A." << endl << endl;

	A.print(cout);

	CIvfMat3d B;
	
	B = A;
	B = 0.0;

	CIvfMat3d C;
	
	C = 0.0;
	C.setRow(1, 1.0, 1.0, 1.0);
	
	cout << endl << "Matrix C." << endl << endl;
	C.print(cout);

	CIvfMat3d D = C.t();
	
	cout << endl << "Matrix D." << endl << endl;
	D.print(cout);

	CIvfMat3d E;

	E.setRow(1, 1.0, 3.0, 2.0);
	E.setRow(2, 5.0, 7.4, 1.5);
	E.setRow(3, -1.0, 2.5, 3.6);
	
	cout << endl << "Matrix E." << endl << endl;
	E.print(cout);
	
	CIvfMat3d F;

	F.setRow(1, 3.0, 1.0, 7.0);
	F.setRow(2, 6.0, 2.4, 2.5);
	F.setRow(3, 2.0, 9.5, 1.6);

	cout << endl << "Matrix F." << endl << endl;
	F.print(cout);

	CIvfMat3d G = E*F;

	cout << endl << "Matrix G=E*F." << endl << endl;
	G.print(cout);

	CIvfVec3d f(1.0, 1.0, 1.0);
	CIvfVec3d g;
	
	g = E*f;

	cout << endl << "Vector g=E*f." << endl << endl;
	g.print(cout);

	CIvfMat3d H = F*F.inv();

	cout << endl << "Matrix g=F*F.inv()." << endl << endl;
	H.print(cout);

	CIvfMat4d X;
	X.setRow(1, 3.0, 4.5, 6.7, 1.2);
	X.setRow(2, 2.0, 7.5, 1.7, 4.2);
	X.setRow(3, 3.0, 5.5, 8.7, 5.2);
	X.setRow(4, 5.0, 4.5, 4.7, 3.2);

	CIvfMat4d Y;

	Y = X*X.inv();

	cout << endl << "Matrix Y=X*X.inv()." << endl << endl;
	Y.print(cout);

	cout << endl << "Rotation matrices Rx, Ry, Rz." << endl << endl;

	CIvfMat4d Rx;
	CIvfMat4d Ry;
	CIvfMat4d Rz;

	Rx.rotateX(45.0*M_PI/360.0); Rx.print(cout); cout << endl;
	Ry.rotateY(45.0*M_PI/360.0); Ry.print(cout); cout << endl;
	Rz.rotateZ(45.0*M_PI/360.0); Rz.print(cout); cout << endl;
	
	CIvfMat4d R;

	cout << endl << "Rotation matrix R." << endl << endl;

	R.rotate(1.0, 0.0, 0.0, 45.0*M_PI/360.0);
	R.print(cout);

	CIvfMat4d T;
	CIvfVec4d p1;
	CIvfVec4d p2;

	cout << endl << "Translation T." << endl << endl;

	p1.setComponents(0.0, 0.0, 0.0, 1.0);
	p1.print(cout); cout << endl;
	T.translate(1.0, 1.0, 1.0);
	T.print(cout); cout << endl;
	p2 = T * p1;
	p2.print(cout);





	return 0;
}
