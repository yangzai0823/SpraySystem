#include "Forms/Login/loginform.h"

#include <QApplication>
void test_traj_line_ik();

int main(int argc, char *argv[])
{
//     test_traj_line_ik();

     QApplication a(argc, argv);
     LoginForm w;
     w.show();
     

     return a.exec();
}
