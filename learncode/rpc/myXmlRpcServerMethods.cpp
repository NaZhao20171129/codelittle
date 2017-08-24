#include <iostream>
#include "myXmlRpcServer.h"
#include "operations.h"

using namespace std;

Add::Add(XmlRpcServer* s) : myXmlRpcServerMethod("Add", s) {};

Void Add::execute(XmlRpcValue & params, XmlRpcValue& result)
{
 operations a(10,12);
 try 
    {
      cout << "Inside Add::execute method\n";
      result = a.add();
    }
  catch(std::exception & stde)
    {
      throw XmlRpcException(stde.what());
    }
}
