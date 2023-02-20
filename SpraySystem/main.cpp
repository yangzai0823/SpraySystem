#include <QApplication>

#include "Forms/Login/loginform.h"
#ifdef LOCALTEST
#include "Forms/Main/testMain.h"
#endif
void test_traj_line_ik();

int main(int argc, char* argv[]) {
    //     test_traj_line_ik();

    QApplication a(argc, argv);
#ifdef LOCALTEST
    // LoginForm w;
    // w.show();

    TestMain* main = new TestMain();
#else
     LoginForm w;
     w.show();
#endif
    return a.exec();
}
