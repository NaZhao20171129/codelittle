class myXmlRpcServerMethod : public XmlRpcServerMethod
{
public:
  myXmlRpcServerMethod
   (const char *name, XmlRpcServer * server):XmlRpcServerMethod(name, server) {}
   virtual void execute(XmlRpcValue & params, XmlRpcValue& result) {assert(0);}
};

class Add:public myXmlRpcServerMethod
{
public:
   Add(XmlRpcServer* s); 
   virtual void execute(XmlRpcValue & params, XmlRpcValue& result);
};
