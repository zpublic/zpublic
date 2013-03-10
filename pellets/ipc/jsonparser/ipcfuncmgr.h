#ifndef ZPUBLIC_JSON_PARSER_IPC_FUNCMGR
#define ZPUBLIC_JSON_PARSER_IPC_FUNCMGR

#include "../../../3rdparty/jsonlib/src/json/json.h"
#include "ipcpublic.h"
#include "ipclocker.h"

namespace zl
{
namespace Ipc
{
    /*
	@ Brief	: 以下是将所有全局函数，类函数，类实例统一维护起来，运行的时候可以用Json通过一个接口调用
	*/
	struct IGlobalFunc 
	{
		virtual ~IGlobalFunc(){}
		virtual const std::string& FuncName(void) = 0;
		virtual int ParamCount(void) = 0;
		virtual std::string Run(Json::Value& param_array) = 0;
	};

	template <class Class>
	class IClassFunc
	{
	public:
		virtual ~IClassFunc(){}
		virtual const std::string& FuncName(void) = 0;
		virtual int ParamCount(void) = 0;
		virtual std::string Run(Class* p_inst, Json::Value& param_array) = 0;
	};

	struct IClassFuncSet 
	{
		virtual ~IClassFuncSet(){}
		virtual std::string& ClassName(void) = 0;
	};

	struct IClassObjectInst
	{
		virtual ~IClassObjectInst(){}
		virtual int InstID(void) = 0;
		virtual std::string Run(const char* func_name, Json::Value& param_array) = 0;
	};

	template <class R>
	class KGlobalFunc_0 : public IGlobalFunc
	{
	public:
		typedef R (*FuncType)(void);

		KGlobalFunc_0(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 0;}
		virtual std::string Run(Json::Value& param_array)
		{
			return RunByType(param_array, Type2Type<R>());
		}
		
	protected:
		template <class T>
		std::string RunByType(Json::Value& param_array, Type2Type<T>)
		{
			R ret_value = func_ptr_();
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Json::Value& param_array, Type2Type<void>)
		{
			func_ptr_();
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class R, class P1>
	class KGlobalFunc_1 : public IGlobalFunc
	{
	public:
		typedef R (*FuncType)(P1);

		KGlobalFunc_1(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 1;}
		virtual std::string Run(Json::Value& param_array)
		{
			return RunByType(param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			R ret_value = func_ptr_(p1.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			func_ptr_(p1.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class R, class P1, class P2>
	class KGlobalFunc_2 : public IGlobalFunc
	{
	public:
		typedef R (*FuncType)(P1, P2);

		KGlobalFunc_2(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 2;}
		virtual std::string Run(Json::Value& param_array)
		{
			return RunByType(param_array, Type2Type<R>());
		}
		
	protected:
		template <class T>
		std::string RunByType(Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			R ret_value = func_ptr_(p1.Value(), p2.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			func_ptr_(p1.Value(), p2.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class R, class P1, class P2, class P3>
	class KGlobalFunc_3 : public IGlobalFunc
	{
	public:
		typedef R (*FuncType)(P1, P2, P3);

		KGlobalFunc_3(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 3;}
		virtual std::string Run(Json::Value& param_array)
		{
			return RunByType(param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			R ret_value = func_ptr_(p1.Value(), p2.Value(), p3.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			func_ptr_(p1.Value(), p2.Value(), p3.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class R, class P1, class P2, class P3, class P4>
	class KGlobalFunc_4 : public IGlobalFunc
	{
	public:
		typedef R (*FuncType)(P1, P2, P3, P4);

		KGlobalFunc_4(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 4;}
		virtual std::string Run(Json::Value& param_array)
		{
			return RunByType(param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			R ret_value = func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class R, class P1, class P2, class P3, class P4, class P5>
	class KGlobalFunc_5 : public IGlobalFunc
	{
	public:
		typedef R (*FuncType)(P1, P2, P3, P4, P5);

		KGlobalFunc_5(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 5;}
		virtual std::string Run(Json::Value& param_array)
		{
			return RunByType(param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
			R ret_value = func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Valule());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
			func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class Class, class R>
	class KClassFunc_0 : public IClassFunc<Class>
	{
	public:
		typedef R (Class::*FuncType)(void);
		KClassFunc_0(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 0;}
		virtual std::string Run(Class* p_inst, Json::Value& param_array)
		{
			return RunByType(p_inst, param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
		{
			R ret_value = (p_inst->*func_ptr_)();
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
		{
			(p_inst->*func_ptr_)();
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class Class, class R, class P1>
	class KClassFunc_1 : public IClassFunc<Class>
	{
	public:
		typedef R (Class::*FuncType)(P1);
		KClassFunc_1(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 1;}
		virtual std::string Run(Class* p_inst, Json::Value& param_array)
		{
			return RunByType(p_inst, param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			R ret_value = (p_inst->*func_ptr_)(p1.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			(p_inst->*func_ptr_)(p1.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class Class, class R, class P1, class P2>
	class KClassFunc_2 : public IClassFunc<Class>
	{
	public:
		typedef R (Class::*FuncType)(P1, P2);
		KClassFunc_2(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 2;}
		virtual std::string Run(Class* p_inst, Json::Value& param_array)
		{
			return RunByType(p_inst, param_array, Type2Type<R>());
		}
		
	protected:
		template <class T>
		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			(p_inst->*func_ptr_)(p1.Value(), p2.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class Class, class R, class P1, class P2, class P3>
	class KClassFunc_3 : public IClassFunc<Class>
	{
	public:
		typedef R (Class::*FuncType)(P1, P2, P3);
		KClassFunc_3(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 3;}
		virtual std::string Run(Class* p_inst, Json::Value& param_array)
		{
			return RunByType(p_inst, param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			(p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class Class, class R, class P1, class P2, class P3, class P4>
	class KClassFunc_4 : public IClassFunc<Class>
	{
	public:
		typedef R (Class::*FuncType)(P1, P2, P3, P4);
		KClassFunc_4(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 4;}
		virtual std::string Run(Class* p_inst, Json::Value& param_array)
		{
			return RunByType(p_inst, param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			(p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	template <class Class, class R, class P1, class P2, class P3, class P4, class P5>
	class KClassFunc_5 : public IClassFunc<Class>
	{
	public:
		typedef R (Class::*FuncType)(P1, P2, P3, P4, P5);
		KClassFunc_5(const char* func_name, FuncType func_ptr)
			: func_name_(func_name), func_ptr_(func_ptr){}

		virtual const std::string& FuncName(void){return func_name_;}
		virtual int ParamCount(void){return 5;}
		virtual std::string Run(Class* p_inst, Json::Value& param_array)
		{
			return RunByType(p_inst, param_array, Type2Type<R>());
		}

	protected:
		template <class T>
		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
			R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Value());
			return KJsonReturn(ret_value).ToString();
		}

		std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
		{
			int n_param_pos_0 = 0;
			Json2Value<P1> p1(param_array[n_param_pos_0]);
			Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
			Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
			Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
			Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
			(p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Value());
			return KJsonReturn().ToString();
		}

	private:
		std::string func_name_;
		FuncType func_ptr_;
	};

	class KGlobalFuncSet
	{
	public:
		~KGlobalFuncSet(void){this->Clear();}

		IGlobalFunc* GetGlobalFunc(const char* func_name, int param_count)
		{
			for (size_t i = 0; i < vec_global_func_.size(); ++i)
			{
				if (vec_global_func_[i]->FuncName() == func_name &&
					vec_global_func_[i]->ParamCount() == param_count)
				{
					return vec_global_func_[i];
				}
			}
			return NULL;
		}

		void Add(IGlobalFunc* p_global_func){vec_global_func_.push_back(p_global_func);}
		void Clear(void){for(size_t i = 0; i < vec_global_func_.size(); ++i) delete vec_global_func_[i]; vec_global_func_.clear();}

	private:
		std::vector<IGlobalFunc*> vec_global_func_;
	};

	template <class Class>
	class KClassFuncSet : public IClassFuncSet
	{
	public:
		KClassFuncSet(const char* class_name) : class_name_(class_name){}
		~KClassFuncSet(void){this->Clear();}

		IClassFunc<Class>* GetClassFunc(const char* func_name, int param_count)
		{
			for (size_t i = 0; i < vec_class_func_.size(); ++i)
			{
				if (vec_class_func_[i]->FuncName() == func_name &&
					vec_class_func_[i]->ParamCount() == param_count)
				{
					return vec_class_func_[i];
				}
			}
			return NULL;
		}

		virtual std::string& ClassName(void){return class_name_;}
		void Add(IClassFunc<Class>* p_class_func){vec_class_func_.push_back(p_class_func);}
		void Clear(void){for(size_t i = 0; i < vec_class_func_.size(); ++i) delete vec_class_func_[i]; vec_class_func_.clear();}

	private:
		std::string class_name_;
		std::vector<IClassFunc<Class>*> vec_class_func_;
	};

	class KAllClassFuncSets
	{
	public:
		~KAllClassFuncSets(void){this->Clear();}

		IClassFuncSet* GetClassFuncSet(const char* class_name)
		{
			for (size_t i = 0; i < vec_class_func_set_.size(); ++i)
			{
				if (vec_class_func_set_[i]->ClassName() == class_name)
				{
					return vec_class_func_set_[i];
				}
			}
			return NULL;
		}

		void Add(IClassFuncSet* p_func_set){vec_class_func_set_.push_back(p_func_set);}
		void Clear(void){for(size_t i = 0; i < vec_class_func_set_.size(); ++i) delete vec_class_func_set_[i]; vec_class_func_set_.clear();}

	private:
		std::vector<IClassFuncSet*> vec_class_func_set_;
	};

	template <class Class>
	class KClassObjectInst : public IClassObjectInst
	{
	public:
		typedef Class TheClass;
		KClassObjectInst(int inst_id, Class* p_inst) : inst_id_(inst_id), p_class_object_inst_(p_inst), p_class_func_set_(NULL){}
		virtual ~KClassObjectInst(){ delete p_class_object_inst_; }

		virtual int InstID(){return inst_id_;}
		virtual std::string Run(const char* func_name, Json::Value& param_array)
		{
			if (!p_class_func_set_) return KJsonReturn(enumRet_FuncNotFound).ToString();
			IClassFunc<Class>* p_class_func = p_class_func_set_->GetClassFunc(func_name, param_array.size());

			if (p_class_func) return p_class_func->Run(p_class_object_inst_, param_array);
			else return KJsonReturn(enumRet_FuncNotFound).ToString();		
		}

		void SetClassFuncSet(KClassFuncSet<Class>* p_func_set){p_class_func_set_ = p_func_set;}

	private:
		int inst_id_;
		Class*	p_class_object_inst_;
		KClassFuncSet<Class>* p_class_func_set_;
	};

	class KClassObjectInstSet
	{
	public:
		~KClassObjectInstSet(){ this->Clear(); }
		void Add(IClassObjectInst* p_inst){KLocker lock(&session_lock_);vec_class_object_.push_back(p_inst);}
		void Clear(void){for(size_t i = 0; i < vec_class_object_.size(); ++i) delete vec_class_object_[i]; vec_class_object_.clear();}

		void Erase(int inst_id)
		{
			KLocker lock(&session_lock_);
			std::vector<IClassObjectInst*>::iterator iter;
			for (iter = vec_class_object_.begin(); iter != vec_class_object_.end(); ++iter)
			{	
				if ((*iter)->InstID() == inst_id)
				{
					IClassObjectInst* pTempInst = *iter;
					vec_class_object_.erase(iter);
					delete pTempInst;
					break;
				}
			}
		}
		IClassObjectInst* GetClassObjectInst(int inst_id)
		{
			KLocker lock(&session_lock_);
			for (size_t i = 0; i < vec_class_object_.size(); ++i)
			{
				if (vec_class_object_[i]->InstID() == inst_id)
				{
					return vec_class_object_[i];
				}
			}
			return NULL;
		}

	private:
		KCriticalSesion	session_lock_;
		std::vector<IClassObjectInst*> vec_class_object_;
	};

	class KInterfaceObjectFactory
	{
	public:
		static KInterfaceObjectFactory& Inst()
		{
			static KInterfaceObjectFactory _interface_object_factory;
			return _interface_object_factory;
		}

		typedef int(*PFNCreateObject)(void);
		void Add(const char* class_name, PFNCreateObject create_func)
		{
			object_create_func_map_.insert(std::make_pair(std::string(class_name), create_func));
		}

		int Create(const char* class_name)
		{
			std::map<std::string, PFNCreateObject>::iterator iter;
			iter = object_create_func_map_.find(class_name);
			if (iter != object_create_func_map_.end()) return iter->second();
			else return enumRet_FuncNotFound;
		}

	protected:
		DISNABLE_CONSTRUCT_AND_DECONSTRUCT(KInterfaceObjectFactory);
		DISNABLE_ASSIGN_OPERATION(KInterfaceObjectFactory);

	private:
		std::map<std::string, PFNCreateObject> object_create_func_map_;
	};

	inline int __CreateObjectByClassName(const char* class_name)
	{
		return KInterfaceObjectFactory::Inst().Create(class_name);
	}

	class KRemoteRunManager
	{
	public:
		static KRemoteRunManager& Inst(void){static KRemoteRunManager s_manager; return s_manager;}

		void Add(IGlobalFunc* p_global_func){global_func_set_.Add(p_global_func);}

		template <class Class>
		void Add(const char* class_name, IClassFunc<Class>* p_class_func)
		{
			IClassFuncSet* pSet = class_func_sets_.GetClassFuncSet(class_name);
			if (pSet)
			{
				KClassFuncSet<Class>* pTempSet = (KClassFuncSet<Class>*)pSet;
				pTempSet->Add(p_class_func);
			}
			else
			{
				KClassFuncSet<Class>* pTempSet = new KClassFuncSet<Class>(class_name);
				pTempSet->Add(p_class_func);
				class_func_sets_.Add(pTempSet);
			}
		}
		void Add(IClassFuncSet* p_func_set){class_func_sets_.Add(p_func_set);}
		void Add(IClassObjectInst* p_inst){class_object_inst_set_.Add(p_inst);}

		std::string Run(const char* json_string)
		{
			Json::Value value;
			std::string str_operation = GetOperation(json_string, value);
			
			if (str_operation == defJsonOperateRun)
			{
				return RunFunc(value);
			}
			else if (str_operation == defJsonOperateAlloc)
			{
				return AllocObject(value);
			}
			else if (str_operation == defJsonOperateDealloc)
			{
				return DeallocObject(value);
			}

			return KJsonReturn(enumRet_Error, "unknown operation").ToString();
		}

		std::string GetOperation(const char* json_string, Json::Value& value)
		{
			Json::Reader reader;

			if (!reader.parse(json_string, value))
			{
				return "";
			}

			Json::Value json_operation = value[defJsonOperation];
			if (!json_operation.isString())
			{
				return "";
			}

			return json_operation.asCString();
		}

		std::string AllocObject(Json::Value& value)
		{
			try
			{
				Json::Value json_class_name = value[defJsonClassName];
				if (json_class_name.isString())
				{
					int inst_id = __CreateObjectByClassName(json_class_name.asCString());
					return KJsonReturn(inst_id).ToString();
				}
				return KJsonReturn(enumRet_Error).ToString();
			}
			catch (KException* e) {
				return KJsonReturn(enumRet_Error, e->what()).ToString();
			}
			catch (std::exception* e)
			{
				return KJsonReturn(enumRet_Error, e->what()).ToString();
			}
			catch (...) {
				return KJsonReturn(enumRet_Error).ToString();
			}
		}

		std::string DeallocObject(Json::Value& value)
		{
			try
			{
				Json::Value json_inst_id = value[defJsonInstID];
				if (json_inst_id.isInt())
				{
					int inst_id = json_inst_id.asInt();
					class_object_inst_set_.Erase(inst_id);
					return KJsonReturn(enumRet_Succeed).ToString();
				}
				return KJsonReturn(enumRet_Error).ToString();
			}
			catch (KException* e) {
				return KJsonReturn(enumRet_Error, e->what()).ToString();
			}
			catch (std::exception* e)
			{
				return KJsonReturn(enumRet_Error, e->what()).ToString();
			}
			catch (...) {
				return KJsonReturn(enumRet_Error).ToString();
			}
		}

		void DeallocObject(int inst_id)
		{
			class_object_inst_set_.Erase(inst_id);
		}

		std::string RunFunc(Json::Value& value)
		{
			try
			{
				Json::Value json_inst_id = value[defJsonInstID];
				Json::Value json_func_name = value[defJsonFuncName];
				Json::Value json_param_array = value[defJsonParamArray];

				if (json_func_name.isNull()) return KJsonReturn(enumRet_FuncNotFound).ToString();

				if (json_inst_id.isNull())
				{
					return RunGlobalFunc(json_func_name.asCString(), json_param_array);
				}
				else
				{
					int inst_id = json_inst_id.asInt();
					IClassObjectInst* p_inst = class_object_inst_set_.GetClassObjectInst(inst_id);
					if (p_inst)
					{
						return p_inst->Run(json_func_name.asCString(), json_param_array);
					}
					else return KJsonReturn(enumRet_FuncNotFound).ToString();
				}
			}
			catch (KException* e) {
				return KJsonReturn(enumRet_Error, e->what()).ToString();
			}
			catch (std::exception* e)
			{
				return KJsonReturn(enumRet_Error, e->what()).ToString();
			}
			catch (...) {
				return KJsonReturn(enumRet_Error).ToString();
			}
		}

		IClassFuncSet* GetClassFuncSet(const char* class_name)
		{
			return class_func_sets_.GetClassFuncSet(class_name);
		}
		
	protected:
		std::string RunGlobalFunc(const char* func_name, Json::Value& param_array)
		{
			IGlobalFunc* p_global_func = global_func_set_.GetGlobalFunc(func_name, param_array.size());
			if (p_global_func)
			{
				return p_global_func->Run(param_array);
			}
			return KJsonReturn(enumRet_FuncNotFound).ToString();
		}

	protected:
		DISNABLE_CONSTRUCT_AND_DECONSTRUCT(KRemoteRunManager);
		DISNABLE_ASSIGN_OPERATION(KRemoteRunManager);

	private:
		KGlobalFuncSet	global_func_set_;
		KAllClassFuncSets class_func_sets_;
		KClassObjectInstSet class_object_inst_set_;
	};

	template <class R>
	struct __KAutoAddGlobalFunc_0
	{
		typedef R (*FuncType)(void);
		__KAutoAddGlobalFunc_0(const char* func_name, FuncType func_ptr)
		{
			KGlobalFunc_0<R>* p_global_func = new KGlobalFunc_0<R>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add(p_global_func);
		}
	};

	template <class R, class P1>
	struct __KAutoAddGlobalFunc_1
	{
		typedef R (*FuncType)(P1);
		__KAutoAddGlobalFunc_1(const char* func_name, FuncType func_ptr)
		{
			KGlobalFunc_1<R, P1>* p_global_func = new KGlobalFunc_1<R, P1>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add(p_global_func);
		}
	};

	template <class R, class P1, class P2>
	struct __KAutoAddGlobalFunc_2
	{
		typedef R (*FuncType)(P1, P2);
		__KAutoAddGlobalFunc_2(const char* func_name, FuncType func_ptr)
		{
			KGlobalFunc_2<R, P1, P2>* p_global_func = new KGlobalFunc_2<R, P1, P2>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add(p_global_func);
		}
	};

	template <class R, class P1, class P2, class P3>
	struct __KAutoAddGlobalFunc_3
	{
		typedef R (*FuncType)(P1, P2, P3);
		__KAutoAddGlobalFunc_3(const char* func_name, FuncType func_ptr)
		{
			KGlobalFunc_3<R, P1, P2, P3>* p_global_func = new KGlobalFunc_3<R, P1, P2, P3>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add(p_global_func);
		}
	};

	template <class R, class P1, class P2, class P3, class P4>
	struct __KAutoAddGlobalFunc_4
	{
		typedef R (*FuncType)(P1, P2, P3, P4);
		__KAutoAddGlobalFunc_4(const char* func_name, FuncType func_ptr)
		{
			KGlobalFunc_4<R, P1, P2, P3, P4>* p_global_func = new KGlobalFunc_4<R, P1, P2, P3, P4>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add(p_global_func);
		}
	};

	template <class R, class P1, class P2, class P3, class P4, class P5>
	struct __KAutoAddGlobalFunc_5
	{
		typedef R (*FuncType)(P1, P2, P3, P4, P5);
		__KAutoAddGlobalFunc_5(const char* func_name, FuncType func_ptr)
		{
			KGlobalFunc_5<R, P1, P2, P3, P4, P5>* p_global_func = new KGlobalFunc_5<R, P1, P2, P3, P4, P5>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add(p_global_func);
		}
	};

	struct  __KAutoAddClass
	{
		typedef int (*PFNCreateObject)(void);
		__KAutoAddClass(const char* class_name, PFNCreateObject func_ptr)
		{
			KInterfaceObjectFactory::Inst().Add(class_name, func_ptr);
		}
	};

	template <class Class, class R>
	struct __KAutoAddClassFunc_0
	{
		typedef R (Class::*FuncType)(void);
		__KAutoAddClassFunc_0(const char* class_name, const char* func_name, FuncType func_ptr)
		{
			KClassFunc_0<Class, R>* p_class_func = new KClassFunc_0<Class, R>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
		}
	};

	template <class Class, class R, class P1>
	struct __KAutoAddClassFunc_1
	{
		typedef R (Class::*FuncType)(P1);
		__KAutoAddClassFunc_1(const char* class_name, const char* func_name, FuncType func_ptr)
		{
			KClassFunc_1<Class, R, P1>* p_class_func = new KClassFunc_1<Class, R, P1>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
		}
	};

	template <class Class, class R, class P1, class P2>
	struct __KAutoAddClassFunc_2
	{
		typedef R (Class::*FuncType)(P1, P2);
		__KAutoAddClassFunc_2(const char* class_name, const char* func_name, FuncType func_ptr)
		{
			KClassFunc_2<Class, R, P1, P2>* p_class_func = new KClassFunc_2<Class, R, P1, P2>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
		}
	};

	template <class Class, class R, class P1, class P2, class P3>
	struct __KAutoAddClassFunc_3
	{
		typedef R (Class::*FuncType)(P1, P2, P3);
		__KAutoAddClassFunc_3(const char* class_name, const char* func_name, FuncType func_ptr)
		{
			KClassFunc_3<Class, R, P1, P2, P3>* p_class_func = new KClassFunc_3<Class, R, P1, P2, P3>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
		}
	};

	template <class Class, class R, class P1, class P2, class P3, class P4>
	struct __KAutoAddClassFunc_4
	{
		typedef R (Class::*FuncType)(P1, P2, P3, P4);
		__KAutoAddClassFunc_4(const char* class_name, const char* func_name, FuncType func_ptr)
		{
			KClassFunc_4<Class, R, P1, P2, P3, P4>* p_class_func = new KClassFunc_4<Class, R, P1, P2, P3, P4>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
		}
	};

	template <class Class, class R, class P1, class P2, class P3, class P4, class P5>
	struct __KAutoAddClassFunc_5
	{
		typedef R (Class::*FuncType)(P1, P2, P3, P4, P5);
		__KAutoAddClassFunc_5(const char* class_name, const char* func_name, FuncType func_ptr)
		{
			KClassFunc_5<Class, R, P1, P2, P3, P4, P5>* p_class_func = new KClassFunc_5<Class, R, P1, P2, P3, P4, P5>(func_name, func_ptr);
			KRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
		}
	};

	template <class Class>
	struct __KAutoAddClassObjectInst 
	{
		__KAutoAddClassObjectInst(int inst_id, Class* p_inst, KClassFuncSet<Class>* p_func_set)
		{
			KClassObjectInst<Class>* p_class_object_inst = new KClassObjectInst<Class>(inst_id, p_inst);
			p_class_object_inst->SetClassFuncSet(p_func_set);
			KRemoteRunManager::Inst().Add(p_class_object_inst);
		}
	};

}
}

#define EXPORT_GLOBAL_FUNC_0(func_name, R) KJsonRun::__KAutoAddGlobalFunc_0<R> func_name##temp_0(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_1(func_name, R, P1) KJsonRun::__KAutoAddGlobalFunc_1<R, P1> func_name##temp_1(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_2(func_name, R, P1, P2) KJsonRun::__KAutoAddGlobalFunc_2<R, P1, P2> func_name##temp_2(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_3(func_name, R, P1, P2, P3) KJsonRun::__KAutoAddGlobalFunc_3<R, P1, P2, P3> func_name##temp_3(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_4(func_name, R, P1, P2, P3, P4) KJsonRun::__KAutoAddGlobalFunc_4<R, P1, P2, P3, P4> func_name##temp_4(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_5(func_name, R, P1, P2, P3, P4, P5) KJsonRun::__KAutoAddGlobalFunc_5<R, P1, P2, P3, P4, P5> func_name##temp_5(#func_name, func_name)

#define DECLARE_EXPORT_INTERFACE(class_name) KJsonRun::__KAutoAddClass class_name##auto_add_class(#class_name, class_name::__##class_name##_CreateObject);
#define EXPORT_CLASS_FUNC_0(class_name, func_name, R) KJsonRun::__KAutoAddClassFunc_0<class_name, R> class_name##func_name##temp_0(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_1(class_name, func_name, R, P1) KJsonRun::__KAutoAddClassFunc_1<class_name, R, P1> class_name##func_name##temp_1(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_2(class_name, func_name, R, P1, P2) KJsonRun::__KAutoAddClassFunc_2<class_name, R, P1, P2> class_name##func_name##temp_2(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_3(class_name, func_name, R, P1, P2, P3) KJsonRun::__KAutoAddClassFunc_3<class_name, R, P1, P2, P3> class_name##func_name##temp_3(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_4(class_name, func_name, R, P1, P2, P3, P4) KJsonRun::__KAutoAddClassFunc_4<class_name, R, P1, P2, P3, P4> class_name##func_name##temp_4(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_5(class_name, func_name, R, P1, P2, P3, P4, P5) KJsonRun::__KAutoAddClassFunc_5<class_name, R, P1, P2, P3, P4, P5> class_name##func_name##temp_5(#class_name, #func_name, &class_name::func_name)

#define ADD_CLASS_OBJECT_INST(class_name, id, p_inst) \
	do {	\
		KJsonRun::KClassFuncSet<class_name>* p_func_set = (KJsonRun::KClassFuncSet<class_name>*)KJsonRun::KRemoteRunManager::Inst().GetClassFuncSet(#class_name); \
		KJsonRun::__KAutoAddClassObjectInst<class_name> class_name##temp_inst(id, p_inst, p_func_set); \
	} while(0)

//namespace KJsonRun {
//class KIdGenerate 
//{
//public:
//	static KIdGenerate& Inst(void)
//	{
//		static KIdGenerate _int_gen;
//		return _int_gen;
//	}
//	int Get(){return ::InterlockedIncrement(&int_value_);}
//
//protected:
//	KIdGenerate():int_value_(0){};
//	~KIdGenerate(){};
//	KIdGenerate(KIdGenerate&){};
//	DISNABLE_ASSIGN_OPERATION(KIdGenerate);
//
//private:
//	long int_value_;
//};
//}

#define DECLARE_CLASS_CREATE_FUNC(class_name) \
	public: \
	static int __##class_name##_CreateObject(void) \
	{ \
		int id; \
		class_name* p_inst = new class_name(); \
		id = (int)p_inst;/*  = KJsonRun::KIdGenerate::Inst().Get(); */\
		ADD_CLASS_OBJECT_INST(class_name, id, p_inst); \
		return id; \
	}

#endif