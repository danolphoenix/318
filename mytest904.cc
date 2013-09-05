#include <map>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/node-container.h"
//以上可以到ns-3.17的build下的ns3文件夹里查找有没有对应的.h文件

//todo：放到头文件里
//#define INFINITYCOST 100000;

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("mytest903");

enum VNodeType {VSwitch,VRouter,Basestation};//node type
enum VirEnv{XEN,VMware,XVM};//virtual environment
enum VOS{Win,Linux,Solaris,Cisco,Juniper};//operating system
enum NStack{IP,ATM,FR,ETH};//network stack
enum VLinkType{VLAN,SONET,WiMax};//link type

namespace ns3 {
             
class myNode:public Node
{
public:
      //VNodeType m_VNodeType;
     // VirEnv    m_VirEnv;
     // VOS       m_VOS;
     // NStack    m_NStack;
     // VLinkType m_VLinkType;
      int       m_capacity;
      //用于记录该物理节点映射不同虚拟节点的cost值
      map< uint32_t,double> m_mapCost;
      
      //myNode(VNodeType m_VNodeType1, VirEnv m_VirEnv1, VOS  m_VOS1,NStack m_NStack1,VLinkType m_VLinkType1,int m_capacity1){
             //m_VNodeType=m_VNodeType1;
            // m_VirEnv=m_VirEnv1;
            // m_VOS=m_VOS1;
            // m_NStack=m_NStack1;
           //  m_VLinkType=m_VLinkType1;
            // m_capacity=m_capacity1;
//}
       myNode(int m_capacity1){ m_capacity=m_capacity1;}

       double getVNCost(Node vnode)
       {
    	   if( m_mapCost.find(vnode.GetId()) != m_mapCost.end())
    	   {
    		   cout<<"the cost of vnode "<< vnode.GetId()<<" to sub node "<< this->GetId()<<" is "<<this->m_mapCost[vnode.GetId()]<<endl;
    		   return this->m_mapCost[vnode.GetId()];
    	   }
    	   else
    	   {
    		   cout<<"Can't find the vnode cost"<<vnode.GetId();
    		   return -1;
    	   }
       }

       //用于初始化的时候设置随机数值作为映射vnode的cost
       //输入：cost：随机产生的cost值 ,vnodeID:虚拟节点的编号
       //输出：0：设置成功，-1：设置失败
       bool setVNCost(double cost, uint32_t vnodeID)
       {

    	   if(m_mapCost.find(vnodeID) != m_mapCost.end())
    	   {
    		   m_mapCost[vnodeID] = cost;
    	       cout<<"set cost of vnode "<< vnodeID<<" to sub node "<< this->GetId() << " successfully"<<" : "<<this->m_mapCost[vnodeID]<<endl;
    	       return 0;
    	   }
    	   else
    	   {
    		   cout<<"set the cost of vnode "<< vnodeID<<" to sub node "<< this->GetId()<< "failed"<<endl;
    		   return -1;
    	   }

       }





       virtual ~myNode(){}
};
}



//todo:用于排序？
void nodesCostSort(NodeContainer container, Node vnode)
{
	 uint32_t nNodes = container.GetN();
	 for (uint32_t i = 0;i < nNodes; ++i)
	 {
	    Ptr<Node> p = container.Get (i);
	    //i->getVNCost ();  // some Node method
	 }

}
//用于初始化各物理节点映射虚拟节点的代价
//input:虚拟节点个数，当前所有节点容器
//output:0:设置成功，-1：设置失败
void initCost(uint32_t vNumber,NodeContainer container)
{
	//先全部设置为INFINITYCOST
	uint32_t nNodes = container.GetN();
	for (uint32_t i = 0;i < nNodes; ++i)
	{
		//D *pd2 = dynamic_cast<D *>(pb);
		//myNode ptr = dynamic_cast<myNode >(*
		Ptr<Node> point = container.Get(i);

		//据说这种做法不安全
		myNode* p = dynamic_cast<myNode *>( &(*container.Get(i))) ;

		for(uint32_t j = 0; j < vNumber; j++)
		{
			//todo:这里最好吧100000作为一个INFINITYCOST的值放到头文件里，等有了头文件再做
			p->m_mapCost.insert(make_pair(j,100000));
		}
	}

	//再生成随机值插入
	for (uint32_t i = 0;i < nNodes; ++i)
		{
		//据说这种做法不安全
			myNode* p = dynamic_cast<myNode *>( &(*container.Get(i))) ;
			//Ptr<myNode> p = (Ptr<>myNode*)container.Get (i);
			for(uint32_t j = 0; j < vNumber; j++)
			{
				double randcost = rand()%10 +1 ;
				p-> setVNCost(randcost,j) ;  // some Node method
			}
		}
}



int main(int argc,char *argv[]){


  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_ALL);
             std::cout<<"create node"<<std::endl;  
             Ptr<myNode> node0 = CreateObject<myNode> (7); 
             Ptr<myNode> node1 = CreateObject<myNode> (6); 
             Ptr<myNode> node2 = CreateObject<myNode> (11);
             Ptr<myNode> node3 = CreateObject<myNode> (8);
             Ptr<myNode> node4 = CreateObject<myNode> (11);
             Ptr<myNode> node5 = CreateObject<myNode> (11);
             Ptr<myNode> node6 = CreateObject<myNode> (11); 
             
             //connect them with a channel
             std::cout<<"nodecontainer1"<<std::endl;
             NodeContainer nc0 = NodeContainer(node0);
             NodeContainer nc1 = NodeContainer(node1);
             NodeContainer nc2 = NodeContainer(node2);
             NodeContainer nc3 = NodeContainer(node3);
             NodeContainer nc4 = NodeContainer(node4);
             NodeContainer nc5 = NodeContainer(node5);
             NodeContainer nc6 = NodeContainer(node6);

             std::cout<<"nodecontainer2"<<std::endl;
             NodeContainer n01 = NodeContainer(nc0, nc1);
             NodeContainer n02 = NodeContainer(nc0, nc2);
             NodeContainer n12 = NodeContainer(nc1, nc2);
             NodeContainer n24 = NodeContainer(nc2, nc4);
             NodeContainer n25 = NodeContainer(nc2, nc5);
             NodeContainer n34 = NodeContainer(nc3, nc4);
             NodeContainer n35 = NodeContainer(nc3, nc5);
             NodeContainer n45 = NodeContainer(nc4, nc5);
             NodeContainer n46 = NodeContainer(nc4, nc6);
             NodeContainer n56 = NodeContainer(nc5, nc6);

             //add ip/tcp stack to all nodes
             std::cout<<"internetstack"<<std::endl;
             InternetStackHelper stack;
             stack.Install (node0);
             stack.Install (node1);
             stack.Install (node2);
             stack.Install (node3);
             stack.Install (node4);
             stack.Install (node5);
             stack.Install (node6);
             
              
             // create point-to-point link
             std::cout<<"point to point"<<std::endl;
             PointToPointHelper pointToPoint1;
             pointToPoint1.SetDeviceAttribute("DataRate", StringValue("30Mbps"));
             pointToPoint1.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_01;
             d_ping_01 = pointToPoint1.Install(n01);

             PointToPointHelper pointToPoint2;
             pointToPoint2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
             pointToPoint2.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_02;
             d_ping_02 = pointToPoint1.Install(n02);

             PointToPointHelper pointToPoint3;
             pointToPoint3.SetDeviceAttribute("DataRate", StringValue("7Mbps"));
             pointToPoint3.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_12;
             d_ping_12 = pointToPoint1.Install(n12);
              
             PointToPointHelper pointToPoint4;
             pointToPoint4.SetDeviceAttribute("DataRate", StringValue("9Mbps"));
             pointToPoint4.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_24;
             d_ping_24 = pointToPoint1.Install(n24);

             PointToPointHelper pointToPoint5;
             pointToPoint5.SetDeviceAttribute("DataRate", StringValue("21Mbps"));
             pointToPoint5.SetChannelAttribute("Delay", StringValue("2ms") );
             NetDeviceContainer d_ping_25;
             d_ping_25 = pointToPoint1.Install(n25);

             PointToPointHelper pointToPoint6;
             pointToPoint6.SetDeviceAttribute("DataRate", StringValue("2Mbps"));
             pointToPoint6.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_34;
             d_ping_34 = pointToPoint1.Install(n34);

             PointToPointHelper pointToPoint7;
             pointToPoint7.SetDeviceAttribute("DataRate", StringValue("3Mbps"));
             pointToPoint7.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_35;
             d_ping_35 = pointToPoint1.Install(n35);

             PointToPointHelper pointToPoint8;
             pointToPoint8.SetDeviceAttribute("DataRate", StringValue("11Mbps"));
             pointToPoint8.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_45;
             d_ping_45 = pointToPoint1.Install(n45);

             PointToPointHelper pointToPoint9;
             pointToPoint9.SetDeviceAttribute("DataRate", StringValue("13Mbps"));
             pointToPoint9.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_46;
             d_ping_46 = pointToPoint1.Install(n46);
 
             PointToPointHelper pointToPoint10;
             pointToPoint10.SetDeviceAttribute("DataRate", StringValue("7Mbps"));
             pointToPoint10.SetChannelAttribute("Delay", StringValue("2ms"));
             NetDeviceContainer d_ping_56;
             d_ping_56 = pointToPoint1.Install(n56);
         

         //add IP addresses
         std::cout<<"add IP address"<<std::endl;
         Ipv4AddressHelper address;
         address.SetBase ("10.1.1.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_01 = address.Assign (d_ping_01);
         std::cout<<"i_ping_01.GetAddress(0):"<<i_ping_01.GetAddress(0)<<std::endl;
         std::cout<<"i_ping_01.GetAddress(1):"<<i_ping_01.GetAddress(1)<<std::endl;

         address.SetBase ("10.1.2.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_02 = address.Assign (d_ping_02);
         std::cout<<"i_ping_02.GetAddress(0):"<<i_ping_02.GetAddress(0)<<std::endl;
         std::cout<<"i_ping_02.GetAddress(1):"<<i_ping_02.GetAddress(1)<<std::endl;

         address.SetBase ("10.1.3.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_12 = address.Assign (d_ping_12);
         std::cout<<"i_ping_12.GetAddress(0):"<<i_ping_12.GetAddress(0)<<std::endl;
         std::cout<<"i_ping_12.GetAddress(1):"<<i_ping_12.GetAddress(1)<<std::endl;


         address.SetBase ("10.1.4.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_24 = address.Assign (d_ping_24);

         address.SetBase ("10.1.5.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_25 = address.Assign (d_ping_25);

         address.SetBase ("10.1.6.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_34 = address.Assign (d_ping_34);

         address.SetBase ("10.1.7.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_35 = address.Assign (d_ping_35);

         address.SetBase ("10.1.8.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_45 = address.Assign (d_ping_45);

         address.SetBase ("10.1.9.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_46 = address.Assign (d_ping_46);

         address.SetBase ("10.1.10.0", "255.255.255.0");
         Ipv4InterfaceContainer i_ping_56 = address.Assign (d_ping_56);

         GlobalRouteManager::AllocateRouterId();
         GlobalRouteManager::BuildGlobalRoutingDatabase();
         GlobalRouteManager::InitializeRoutes();

         //在开始仿真之前给每个物理节点上面制造一堆映射虚拟节点需要用的cost。
         uint32_t vNumber = 4;
         initCost(vNumber, nc0.GetGlobal());


         //打印出设置的cost值
         NodeContainer globalContainer = nc0.GetGlobal();
         uint32_t nNodes = globalContainer.GetN();
     	for (uint32_t i = 0;i < nNodes; ++i)
     	{

     		Ptr<Node> point = globalContainer.Get(i);
     		//据说这种做法不安全
     		myNode* p = dynamic_cast<myNode *>( &(*globalContainer.Get(i))) ;
     		cout<< "sub node "<< p->GetId()<<endl;

     		for(uint32_t j = 0; j < vNumber; j++)
     		{

     			cout<< "vnode: "<< j <<" cost:"<< p->m_mapCost[j]<<endl;
     		}
     	}





         //add Application
         //add Server
         UdpEchoServerHelper echoServer(9999);
         ApplicationContainer serverApps=echoServer.Install(node0);
         serverApps.Start(Seconds(1.0));
         serverApps.Stop(Seconds(10.0));

         //add Client
         UdpEchoClientHelper echoClient(i_ping_01.GetAddress(0),9999);
         echoClient.SetAttribute("MaxPackets",UintegerValue(1));
         echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
         echoClient.SetAttribute("PacketSize",UintegerValue(1));

         ApplicationContainer clientApps=echoClient.Install(node6);
         clientApps.Start(Seconds(2.0))	;
         clientApps.Stop(Seconds(10.0));

         Simulator::Run();
         Simulator::Destroy();
         return 0;
}
         


