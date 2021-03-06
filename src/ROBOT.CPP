#include "Robot.h"
#define PI 3.14159265
///Copyright (C) <2017>  <Eliseo Rivera> curso.tareas@gmail.com
Robot::Robot()
{
    z3=0;
   theta1=0;theta2=0;theta3=0; theta4=0; theta5=0; theta6=0,theta7=0;
   THx.identity(4);
   THy.identity(4);
   THz.identity(4);
  int d=4;
  TH.identity(4);
  J.zero(4,4);

  ///home
  Xg=-5.7609;
 Yg=11.2138;
 Zg=4.11889;

}
Matrix Robot::F(float t1, float t2,float z, float t4){
///los argumentos estan radianes
Matrix f(4,1);
f.entry(0,0)=sin(t1)-Xg-3*(sin(t4))*((0.5)*sqrt(2)*cos((8/9.0)*PI+t2)*cos(t1)+(0.5)*sqrt(2)*sin((8/9.0)*PI+t2)*cos(t1))-(sin(t1))*(z+9.5)+5.5*cos((8/9.0)*PI+t2)*cos(t1)-3*cos(t4)*sin(t1);
f.entry(1,0)=(cos(t1))*(z+9.5)-cos(t1)-3*(sin(t4))*((0.5)*sqrt(2)*cos((8/9.0)*PI+t2)*sin(t1)+(0.5)*sqrt(2)*sin((8/9.0)*PI+t2)*sin(t1))-Yg+5.5*cos((8/9.0)*PI+t2)*sin(t1)+3*cos(t1)*cos(t4);
f.entry(2,0)=6.0-5.5*sin((8/9.0)*PI+t2)-3*(sin(t4))*((0.5)*sqrt(2)*cos((8/9.0)*PI+t2)-(0.5)*sqrt(2)*sin((8/9.0)*PI+t2))-Zg;
f.entry(3,0)=0;
return f;
};

void Robot::DefinirJacobian(float t1, float t2,float z, float t4){


J.entry(0,0)=0.53033*sin(t1+t2-t4+2.7925)-0.53033*sin(t1-t2-t4-2.7925)+0.53033*sin(t1-t2+t4-2.7925)-0.53033*sin(t1+t2+t4+2.7925)- 8.5*cos(t1)-z*cos(t1)-3.0*cos(t1)*cos(t4)- 5.5*sin(t1)*cos(t2+2.7925)+ 2.1213*sin(t1)*sin(t4)*cos(t2+2.7925);
J.entry(0,1)=2.1213*cos(t1)*sin(t4)*sin(t2+2.7925)- 2.1213*cos(t1)*sin(t4)*cos(t2+2.7925)- 5.5*cos(t1)*sin(t2+2.7925);
J.entry(0,2)=-sin(t1);
J.entry(0,3)=(1.5)*cos(t1-t4)-(1.5)*cos(t1+t4)-(1.5)*sqrt(2)*cos((7/(18.0))*PI+t2)*cos(t1)*cos(t4)+(1.5) *sqrt(2.0)*sin((7/(18.0))*PI+t2)*cos(t1)*cos(t4);

J.entry(1,0)= 5.5*cos(t1)*cos(t2+2.7925)- 1.0*z*sin(t1)-3.0*cos(t4)*sin(t1)-8.5*sin(t1)- 2.1213*cos(t1)*sin(t4)*cos(t2+2.7925)- 2.1213*cos(t1)*sin(t4)*sin(t2+2.7925);
J.entry(1,1)= 2.1213*sin(t1)*sin(t4)*sin(t2+2.7925)- 2.1213*sin(t1)*sin(t4)*cos(t2+2.7925)- 5.5*sin(t1)*sin(t2+2.7925);
J.entry(1,2)=cos(t1);
J.entry(1,3)=(1.5)*sqrt(2.0)*sin((7/(18.0))*PI+t2)*cos(t4)*sin(t1)-(1.5)*sqrt(2.0)*cos((7/(18.0))*PI+t2)*cos(t4)*sin(t1)-3.0*cos(t1)*sin(t4);

J.entry(2,0)=0;
J.entry(2,1)=1.0607*cos(t4-t2-2.7925)- 5.5*cos(t2+2.7925)- 1.0607*cos(t2+t4+2.7925)+ 2.1213*sin(t4)*cos(t2+2.7925);
J.entry(2,2)=0;
J.entry(2,3)= (1.5)*sqrt(2)*(cos(t4))*(cos((7/(18.0))*PI+t2)+sin((7/(18.0))*PI+t2));

///ultima fila del jacobiano
J.entry(3,0)=0.0001;  ///solo para efectos de convergencia
J.entry(3,1)=0.0001;
J.entry(3,2)=0.0001;
J.entry(3,3)=0.0001;


}
void Robot::Newton_Rapshon(float &q1,float &q2,float &q3,float &q4){


Matrix q(4,1),dq(4,1);
/// configuración home

q.entry(0,0)=q1;
q.entry(1,0)=q2;
q.entry(2,0)=q3;
q.entry(3,0)=q4;
int i=0;
do{

DefinirJacobian(q.entry(0,0),q.entry(1,0),q.entry(2,0),q.entry(3,0));
dq=(-1.0)*J.pseudoinversa(i)*F(q.entry(0,0),q.entry(1,0),q.entry(2,0),q.entry(3,0));
if (i<15) q=q+dq;else break;


}while (F(q.entry(0,0),q.entry(1,0),q.entry(2,0),q.entry(3,0)).column_2norm(0)>0.0001);

if (i<15){
q1=(180.0/PI)*q.entry(0,0);
q2=(180.0/PI)*q.entry(1,0);
q3=q.entry(2,0);
q4=(180.0/PI)*q.entry(3,0);

AplicarTHz( theta1+q1, {0,0,6});     //b1
THList[2]=THz;

AplicarTHz( theta2+q2, {0,0,-1});     //b2
THList[4]=THz;

AplicarTHz( theta3, {0,0,z3+q3});     //b3
THList[6]=THz;

AplicarTHz( theta4+q4, {0,0,0});     //b4
THList[8]=THz;

}


else{

cout<<"configuracion no alcanzada, cambie de dirección"<<endl;

}

return ;
}
void Robot::PrepararNewtonRapshon(){
  float q1,q2,q3,q4;
q1=0;
q2=0;
q3=0;
q4=0;

 Newton_Rapshon( q1,q2,q3,q4);
/*mas codigo donde se utilizen qi*/


}
void Robot::parametrica(){

Xg=-5.7609;
Yg=11.2138;
Zg=4.11889+t;


}
Robot::~Robot()
{

delete base;
delete e1;
delete e2;
delete e3;
delete e4;
delete e5;

    //dtor
}

void Robot::inicializar(){
base=new modelo3D();
e1=new modelo3D();
e2=new modelo3D();
e3=new modelo3D();
e4=new modelo3D();
e5=new modelo3D();



base->leer("base.stl");
e1->leer("e1.stl");
e2->leer("e2.stl");
e3->leer("e3.stl");
e4->leer("e4.stl");
e5->leer("e5.stl");



modelos.push_back(base);
modelos.push_back(e1);
modelos.push_back(e2);
modelos.push_back(e3);
modelos.push_back(e4);
modelos.push_back(e5);

}

void Robot::configurarTH(){
AplicarTHz(0,{0,0,0}); //base
THList.push_back(THz);
AplicarTHx(0,{0,0,0}); //base
THList.push_back(THx);

AplicarTHz(0,{0,0,6}); //b1
THList.push_back(THz);
AplicarTHx(-90,{0,0,0}); //b1
THList.push_back(THx);

theta2=160;
AplicarTHz(theta2,{0,0,-1}); //b2
THList.push_back(THz);
AplicarTHx(0,{5.5,0,0}); //b2
THList.push_back(THx);

z3=9.5;
theta3=-45;
AplicarTHz(theta3,{0,0,z3}); //b3
THList.push_back(THz);
AplicarTHx(90,{0,0,0}); //b3
THList.push_back(THx);
theta4=180;

AplicarTHz(theta4,{0,0,0}); //b4
THList.push_back(THz);
AplicarTHx(90,{0,0,0}); //b4
THList.push_back(THx);

AplicarTHz(0,{0,0,3.0}); //b5
THList.push_back(THz);
AplicarTHx(0,{0,0,0}); //b5
THList.push_back(THx);

PrepararNewtonRapshon();

}
void Robot::renderizar(){


TH.resetIdentity();

modelo3D *model;

for (int m=0;m<modelos.size();m++){

    model=modelos[m];
    TH=TH* THList[2*m+0]*THList[2*m+1];


vector3d ux,uy,uz,O;
ux={1,0,0};
uy={0,1,0};
uz={0,0,1};

Matrix ux4(ux,1),uy4(uy,1),uz4(uz,1),O4(O,1);


ux4=TH*ux4-TH*O4;
uy4=TH*uy4-TH*O4;
uz4=TH*uz4-TH*O4;
O4=TH*O4;


ux={ux4.aij[0][0],ux4.aij[1][0],ux4.aij[2][0]};
uy={uy4.aij[0][0],uy4.aij[1][0],uy4.aij[2][0]};
uz={uz4.aij[0][0],uz4.aij[1][0],uz4.aij[2][0]};
O={O4.aij[0][0],O4.aij[1][0],O4.aij[2][0]};

//if (m<2){
        Drawarrow3D(O,O+4*ux,{1,0.1,0.2},0.03,0.1);
         Drawarrow3D(O,O+4*uy,{.1,1,0.2},0.03,0.1);
         Drawarrow3D(O,O+4*uz,{0.1,0.2,1},0.03,0.1);
       //  }
         glColor4f(fabs(cos(m*PI/modelos.size())),fabs(sin(20*(m-5)*PI/modelos.size())),0.2, 0.5);

glEnable(GL_BLEND);
 glBegin(GL_TRIANGLES);

  glFrontFace(GL_FRONT_AND_BACK);
    for (int i=0;i<model->ntriangles;i++){

vector3d v1=model->triangulos[i].vertices[0];   //posiciones locales
vector3d v2=model->triangulos[i].vertices[1];
vector3d v3=model->triangulos[i].vertices[2];
Matrix v14(v1,1),v24(v2,1),v34(v3,1);

v14=TH*v14;
v24=TH*v24;
v34=TH*v34;
v1={v14.entry(0,0),v14.entry(1,0),v14.entry(2,0)};
v2={v24.entry(0,0),v24.entry(1,0),v24.entry(2,0)};
v3={v34.entry(0,0),v34.entry(1,0),v34.entry(2,0)};



Matrix N(4,1),d14(4,1),d24(4,1);
d14=v24-v14;
d24=v34-v14;
vector3d d1,d2,n;
d1={d14.entry(0,0),d14.entry(1,0),d14.entry(2,0)};
d2={d24.entry(0,0),d24.entry(1,0),d24.entry(2,0)};
n=d1*d2;  ///devuelve el producto vectorial
n.normalize();



        glNormal3f(n.x,n.y,n.z);
        glVertex3f(v1.x,v1.y,v1.z);
        glVertex3f(v2.x,v2.y,v2.z);
        glVertex3f(v3.x,v3.y,v3.z);
    }
glEnd();
// }
 glDisable(GL_BLEND);


///DIBUJAR EJES


//}
}

}




void Robot::DefinirTHx(float dtheta, vector3d d){

THx.aij[0][0]=1;
THx.aij[0][1]=0;
THx.aij[0][2]=0;
THx.aij[0][3]=d.x;

THx.aij[1][0]=0;
THx.aij[1][1]=cos(dtheta);
THx.aij[1][2]=-sin(dtheta);
THx.aij[1][3]=d.y;

THx.aij[2][0]=0;
THx.aij[2][1]=sin(dtheta);
THx.aij[2][2]=cos(dtheta);
THx.aij[2][3]=d.z;

THx.aij[3][0]=0;
THx.aij[3][1]=0;
THx.aij[3][2]=0;
THx.aij[3][3]=1;

}
void Robot::DefinirTHy(float dtheta, vector3d d){


THy.aij[0][0]=cos(dtheta);
THy.aij[0][1]=0;
THy.aij[0][2]=sin(dtheta);

THy.aij[1][0]=0;
THy.aij[1][1]=1;
THy.aij[1][2]=0;

THy.aij[2][0]=-sin(dtheta);
THy.aij[2][1]=0;
THy.aij[2][2]=cos(dtheta);

THy.aij[3][0]=0;
THy.aij[3][1]=0;
THy.aij[3][2]=0;
THy.aij[3][3]=1;

THy.aij[0][3]=d.x;
THy.aij[1][3]=d.y;
THy.aij[2][3]=d.z;
}
void Robot::DefinirTHz(float dtheta, vector3d d){

THz.aij[0][0]=cos(dtheta);
THz.aij[0][1]=-sin(dtheta);
THz.aij[0][2]=0;
THz.aij[0][3]=d.x;

THz.aij[1][0]=sin(dtheta);
THz.aij[1][1]=cos(dtheta);
THz.aij[1][2]=0;
THz.aij[1][3]=d.y;

THz.aij[2][0]=0;
THz.aij[2][1]=0;
THz.aij[2][2]=1;
THz.aij[2][3]=d.z;

THz.aij[3][0]=0;
THz.aij[3][1]=0;
THz.aij[3][2]=0;
THz.aij[3][3]=1;

}

void Robot::AplicarTHx(float theta, vector3d d){
theta=theta*PI/180.0;

DefinirTHx(theta,d);

}
void Robot::AplicarTHy(float theta, vector3d d){
theta=theta*PI/180.0;
DefinirTHy(theta,d);

}
void Robot::AplicarTHz(float theta, vector3d d){
theta=theta*PI/180.0;
DefinirTHz(theta,d);

}

void Robot::Drawarrow3D( vector3d A,  vector3d B, vector3d color, double cota1,double R)
{

double color1,color2,color3,a,b,c,d,e;



color1=color.x;//abs(color1/255);
color2=color.y;//abs(color2/255);
color3=color.z;//abs(color3/255);

glColor3f( color1,color2, color3);

vector3d n=B-A,np,vertex[10],normallight;
n.normalize();
if(n.z!=0)np={1,1,(-1/n.z)*(n.x+n.y)};
else if(n.y!=0)np={1,(-1/n.y)*(n.x+n.z),1};
else np={(-1/n.x)*(n.y+n.z),1,1};

np.normalize();
vertex[0]=R*np;
vertex[2]=R*(n*np).normalize();
vertex[1]=R*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
vertex[4]=R*(n*vertex[2]).normalize();
vertex[3]=R*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
vertex[6]=R*(n*vertex[4]).normalize();
vertex[5]=R*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
vertex[7]=R*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
vertex[8]=vertex[0];
vertex[9]=vertex[1];
int nx=8;
double d_thetha,fraccion=0.1,radioflecha=R+.7*R;
d_thetha=2.0f*PI/nx;


  ///tubos
 glBegin( GL_TRIANGLE_STRIP );

         for(int i=0;i<9;i++)
               {

normallight=n*(vertex[i-1]-vertex[i+1]);
normallight.normalize();
glNormal3f(normallight.x, normallight.y, normallight.z);
                 glVertex3f(vertex[i].x+A.x,vertex[i].y+A.y,vertex[i].z+A.z);

glVertex3f(vertex[i].x+B.x-fraccion*(B.x-A.x),vertex[i].y+B.y-fraccion*(B.y-A.y),vertex[i].z+B.z-fraccion*(B.z-A.z));

    // top face

                }

glEnd();



//flecha
vertex[0]=radioflecha*np;
vertex[2]=radioflecha*(n*np).normalize();
vertex[1]=radioflecha*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
vertex[4]=radioflecha*(n*vertex[2]).normalize();
vertex[3]=radioflecha*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
vertex[6]=radioflecha*(n*vertex[4]).normalize();
vertex[5]=radioflecha*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
vertex[7]=radioflecha*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
vertex[8]=vertex[0];
vertex[9]=vertex[1];
vector3d Ap(B-fraccion*(B-A));



 glBegin( GL_TRIANGLE_STRIP );  //flecha

         for(int i=0;i<9;i++)
               {

normallight=n*(vertex[i-1]-vertex[i+1]);
normallight.normalize();
glNormal3f(normallight.x, normallight.y, normallight.z);
                 glVertex3f(vertex[i].x+Ap.x,vertex[i].y+Ap.y,vertex[i].z+Ap.z);


glNormal3f(n.x, n.y, n.z);
glVertex3f(Ap.x+fraccion*(B-A).x,Ap.y+fraccion*(B-A).y,Ap.z+fraccion*(B-A).z);

    // top face

                }

glEnd();


}
