#ifndef COMPONENT_HANDLER_H
#define COMPONENT_HANDLER_H

template <typename T>
class tgComponentHandler {
public:
	tgComponentHandler() 
		: m_component(nullptr)
	{}
	tgComponentHandler(T *comp) 
		: m_component(comp)
	{}

	T* operator ->() const {
		return m_component;
	}

	operator bool() const {
		return isValid();
	}

	T& get() {
		return *m_component;
	}

	bool isValid() const {
		return m_component != nullptr;
	}
private:
	T *m_component;
};

#endif
