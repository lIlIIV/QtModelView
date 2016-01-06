//
//  dispatch.hh
//  ipso-hello
//
//  Copyright ipsosanté 2014. All rights reserved.
//

#ifndef TOOLS_DISPATCH_HH
#define TOOLS_DISPATCH_HH

#include <QThread>
#include <functional>
#include <QObject>
#include <QPointer>

typedef std::function<void()> FnBlock;

class WorkerClass : public QObject
{
  Q_OBJECT

public:

  WorkerClass(QThread *thread)
  {
    moveToThread(thread);
  }

public slots:

  void doWork(FnBlock block)
  {
    block();
  }

};

void dispatchAsync(FnBlock block);
void dispatchAsync(QThread* thread, FnBlock block);

void dispatchAsyncMain(FnBlock block);
void dispatchSyncMain(FnBlock block);

void dispatchAsync(QPointer<QObject> widget, FnBlock block);
void dispatchAsync(QPointer<QObject> widget, QThread* thread, FnBlock block);

void dispatchAsyncMain(QPointer<QObject> widget, FnBlock block);
void dispatchSyncMain(QPointer<QObject> widget, FnBlock block);

// void dispatchAsyncMain(QPointer<QObject> widget, FnBlock block);
// void dispatchSyncMain(QPointer<QObject> widget, FnBlock block);

#endif // TOOLS_DISPATCH_HH
