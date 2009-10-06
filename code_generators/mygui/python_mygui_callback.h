#ifndef _python_mygui_callback_H_
#define _python_mygui_callback_H_

class EventCallback
{
public:
	EventCallback() : mSubscriber(0)
	{
	}

	EventCallback(PyObject *subscriber, std::string &_method)
	{
		mSubscriber = subscriber;
		mMethod = _method;
	}
	
	~EventCallback()
	{
	}
	
	bool operator() () const
	{
		return true;
	}
	
	template<typename T1>
	void callback(T1 v1)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1));
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1));
	}
	
	template<typename T1, typename T2>
	void callback(T1 v1, T2 v2)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1), boost::ref(v2));
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1), boost::ref(v2));
	}
	
	template<typename T1, typename T2, typename T3>
	void callback(T1 v1, T2 v2, T3 v3)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1), boost::ref(v2), boost::ref(v3));
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1), boost::ref(v2), boost::ref(v3));
	}
	
	template<typename T1, typename T2, typename T3, typename T4>
	void callback(T1 v1, T2 v2, T3 v3, T4 v4)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1), boost::ref(v2), boost::ref(v3), boost::ref(v4));
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1), boost::ref(v2), boost::ref(v3), boost::ref(v4));
	}	
	
	template<typename T1, typename T2, typename T3>
	void callbackP_R_R(T1 v1, T2 v2, T3 v3)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1), v2, v3);
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1), v2, v3);
	}
	
	template<typename T1, typename T2>
	void callbackP_R(T1 v1, T2 v2)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1), v2);
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1), v2);
	}
	
	template<typename T1, typename T2, typename T3, typename T4>
	void callbackP_R_R_R(T1 v1, T2 v2, T3 v3, T4 v4)
	{
		if (mMethod.length() > 0 )
			boost::python::call_method<void>(mSubscriber, mMethod.c_str(), boost::ref(v1), v2, v3, v4);
		else
			boost::python::call<void>(mSubscriber, boost::ref(v1), v2, v3, v4);
	}
	
protected:	
	PyObject *mSubscriber;
	std::string mMethod;
};

#endif