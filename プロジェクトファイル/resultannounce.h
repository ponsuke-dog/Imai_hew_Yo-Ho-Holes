#ifndef  RESULTANNOUNCE_H
#define  RESULTANNOUNCE_H

#include <vector>
class Controller;

void ResultAnnounceInitialize();
void ResultAnnounceFinalize();
void ResultAnnounceUpdate(std::vector<Controller*> con);
void ResultAnnounceDraw();

bool GetResultAnnouncePushFlags();


#endif // ! RESULTANNOUNCE_H
