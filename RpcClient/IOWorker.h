#ifndef _IOWORKER_H_
#define _IOWORKER_H_

#include "RpcClientGlobal.h"

/************************************************************************
��  �ܣ�libevent �������ɶ���ص��˺���
��  ������libevent event_callback_fn����
����ֵ����
************************************************************************/
void notifiedCallback(evutil_socket_t fd, short events, void *pArg);

/************************************************************************
��  �ܣ�libevent bufferevent���뻺�����ɶ���ص��˺���
��  ������libevent bufferevent_data_cb����
����ֵ����
************************************************************************/
void readCallback(bufferevent *pBufEv, void *pArg);

/************************************************************************
��  �ܣ�libevent bufferevent�Ϸ����ɶ�����д֮����¼���ص��˺���
��  ������libevent bufferevent_event_cb����
����ֵ����
************************************************************************/
void eventCallback(bufferevent *pBufEv, short events, void *pArg);

//IOWorker������һ���̣߳��¼�ѭ���������߳���
class IOWorker
{
public:
	/************************************************************************
	��  �ܣ����췽��
	��  ����
		fds�����룬IOWorker��Ӧ��socketpair
		queueMaxSize�����룬������󳤶�
		heartbeatInterval�����룬������PING�����ķ�������
	����ֵ����
	************************************************************************/
	IOWorker(evutil_socket_t *fds, unsigned int queueMaxSize, timeval heartbeatInterval);

	/************************************************************************
	��  �ܣ���������
	��  ������
	����ֵ����
	************************************************************************/
	~IOWorker();

	/************************************************************************
	��  �ܣ�����IOWorker
	��  ������
	����ֵ����
	************************************************************************/
	void start();

	/************************************************************************
	��  �ܣ�����IO����֪ͨ��IO�������IOWorker������
	��  ������
	����ֵ����
	************************************************************************/
	void handleIOTask();

	/************************************************************************
	��  �ܣ���������֪ͨ
	��  ������
	����ֵ����
	************************************************************************/
	void handleEnd();

	/************************************************************************
	��  �ܣ�����bufferevent���뻺��������
	��  ����
		pConn�����룬����ָ��
	����ֵ����
	************************************************************************/
	void handleRead(Conn *pConn);

	/************************************************************************
	��  �ܣ�����bufferevent�����ӳɹ��¼�
	��  ����
		pConn�����룬����ָ��
	����ֵ����
	************************************************************************/
	void handleConnected(Conn *pConn);

	/************************************************************************
	��  �ܣ�����bufferevent�ĳ�ʱ�¼�
	��  ����
		pConn�����룬����ָ��
	����ֵ����
	************************************************************************/
	void handleTimeout(Conn *pConn);

	/************************************************************************
	��  �ܣ���������
	��  ����
		pConn�����룬����ָ��
	����ֵ��
		true�����ӳɹ�
		false������ʧ��
	************************************************************************/
	bool connect(Conn *pConn);

	/************************************************************************
	��  �ܣ��ͷ�����
	��  ����
		pConn�����룬����ָ��
	����ֵ����
	************************************************************************/
	void freeConn(Conn *pConn);

	/************************************************************************
	��  �ܣ���ȡIOWorker�ķ�æ�̶ȣ��Ե�ǰIOWorker m_mapConn��size��ʾ��æ�̶�
	��  ������
	����ֵ��IOWorker�ķ�æ�̶�
	************************************************************************/
	unsigned int getBusyLevel();

	/************************************************************************
	��  �ܣ���ȡIOWorker��socketpair��д��������
	��  ������
	����ֵ��IOWorker��socketpair��д��������
	************************************************************************/
	evutil_socket_t getNotify_fd();

	/************************************************************************
	��  �ܣ���������
	��  ������
	����ֵ��Connָ�룬������ʧ�ܣ�����id������������NULL
	************************************************************************/
	Conn *genConn();

	//���У����ڷ���IO����
	SyncQueue<IOTask> m_queue;

private:
	/************************************************************************
	��  �ܣ��߳�������
	��  ������
	����ֵ����
	************************************************************************/
	void threadMain(evutil_socket_t notified_fd);

	/************************************************************************
	��  �ܣ������Ͽ����ӵ�����
	��  ����
		connId�����룬����id
	����ֵ����
	************************************************************************/
	void handleDisconnect(unsigned int connId);

	/************************************************************************
	��  �ܣ��������õ�����
	��  ����
		pCall�����룬����ָ��
	����ֵ����
	************************************************************************/
	void handleCall(Call *pCall);

	/************************************************************************
	��  �ܣ�rpc�ص�
	��  ����
		pClosure�����룬���ڻص��û�������Closureָ��
		sync_write_fd�����룬����ͬ�����õ�socketpairд��������
	����ֵ����
	************************************************************************/
	void rpcCallback(google::protobuf::Closure *pClosure, evutil_socket_t sync_write_fd);

	//�߳�
	boost::thread m_thd;
	//socketpair����������
	evutil_socket_t m_notified_fd;
	//socketpairд��������
	evutil_socket_t m_notify_fd;
	//Ψһ����id������
	UniqueIdGenerator<unsigned int> m_connIdGen;
	//�������ӣ�map<����id, ����ָ��>
	map<unsigned int, Conn *> m_mapConn;
	//event_baseָ��
	event_base *m_pEvBase;
	//������PING�����ķ�������
	timeval m_heartbeatInterval;
	//IOWorker�Ƿ��Ѿ���ʼ����
	bool m_bStarted;
	//IOWorker�Ƿ��Ѿ���ʼ����
	bool m_bEnded;
};

#endif