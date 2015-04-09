/*
 * ComboBox.h
 *
 *  Created on: 19.5.2012
 *      Author: Morphe
 */

#ifndef COMBOBOX_H_
#define COMBOBOX_H_

#include "DropDownCB.h"
#include "DropDownListCB.h"
#include "SimpleCB.h"
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>


namespace ComboBox {

template <class T>
struct _ConstNamer {
	std::wstring operator() (const T& data) const { return (std::wstring)data; }
};


/// Provides a template for selection window (based on ComboBox classes)
template <class _ComboBoxT, typename T, class _Namer,
	class _Cmp = std::less<T>, class _Eq = std::equal_to<T>>
class Selector : public _ComboBoxT {
public:
	Selector(int x, int y, int width, int height, Window& parent) :
		_ComboBoxT(x, y, width, height, parent) {
	}
	~Selector() {}

	void setItems() {
		dataContainer.clear();
		resetList();
	}

	template <typename _ContainerT>
	void setItems(_ContainerT container) {
		dataContainer.clear();
		for(auto& i : container)
			dataContainer.push_back(_Mapper(i));
		this->setCurSelNum(-1); // Deselect item
		resetList();
	}

	void setItems(std::initializer_list<T> list) {
		setItems<std::initializer_list<T>>(list);
	}

	void addItem(const T& item) {
		dataContainer.push_back(_Mapper(item));
		resetList();
	}

	void resetList() {
        resetting_ = true;
		std::wstring selectionUpdated;
		if(this->getCurSelNum() >= 0)
			selectionUpdated = (std::wstring) dataContainer[this->getCurSelNum()];
		sort(dataContainer.begin(), dataContainer.end());
		this->setStrings(dataContainer);
		this->setCurSelNum(this->findString(selectionUpdated));
        resetting_ = false;
	}

	void setOnItemChange(WndEventExtBase<T> *onItemChange) {
		onItemChange_ = onItemChange;
	}

	class _Mapper {
		_Namer namer_;
		_Cmp cmp;
		_Eq eq;
		T data_;
	public:
		_Mapper(const T& data) : data_(data) {
		}

		~_Mapper() {
		}

		bool operator<(const _Mapper& m) const {
			return cmp(data_, m.data_);
		}

		bool operator==(const _Mapper& m) const {
			return eq(data_, m.data_);
		}

		T getData() const {
			return data_;
		}

		std::wstring getName() const {
			return namer_(data_);
		}

		// To comply with ComboBoxWnd::setStrings
		explicit operator std::wstring() const {
			return getName();
		}

	};

protected:
	int onSelChange(int index) {
        if (!resetting_) {
    		if(index == -1)
    			onItemChange_(*this, T());
    		else
    			onItemChange_(*this, dataContainer[index].getData());
        }
		return _ComboBoxT::onSelChange(index);
	}

private:
    bool resetting_ = false;
	std::vector<_Mapper> dataContainer;
	WndEventExtCaller<T> onItemChange_;
};

template <class _ComboBoxT, typename T>
class NamedSelector : public _ComboBoxT {
public:
	NamedSelector(int x, int y, int width, int height, Window& parent) :
		_ComboBoxT(x, y, width, height, parent) {
	}
	~NamedSelector() {}

	void setItems() {
		selector_.clear();
		this->setStrings(selector_);
	}

	bool contains(const std::wstring &name) {
		return selector_.count(_Mapper(name, T()));
	}

	void setItems(std::initializer_list<
			std::pair<std::wstring, T>> l) {
		selector_.clear();
		for(auto& i : l)
			selector_.insert(i);
		this->setStrings(selector_);
	}

	void addItem(const std::wstring& name, const T& item) {
		selector_.insert(_Mapper(name, item));
		this->setStrings(selector_);
	}

	void removeItem(const std::wstring& name) {
		selector_.erase(_Mapper(name, T()));
		this->setStrings(selector_);
	}

	void setOnItemChange(WndEventExtBase<T> *onItemChange) {
		onItemChange_ = onItemChange;
	}

	class _Mapper {
		std::wstring name_;
		T data_;
	public:
		_Mapper(const std::wstring& name, const T& data) :
			name_(name), data_(data) {
		}

		_Mapper(_Mapper&& m) :
			name_(m.name_), data_(m.data_) {
			m.data_ = T();
		}

		_Mapper(const std::pair<std::wstring, T>& c) :
			name_(c.first), data_(c.second){
		}

		_Mapper(const std::wstring& name) : name_(name) {
		}

		bool operator<(const _Mapper& m) const {
			return getName() < m.getName();
		}

		bool operator==(const _Mapper& m) const {
			return getName() == m.getName();
		}

		T getData() const {
			return data_;
		}

		std::wstring getName() const {
			return name_;
		}

		void setName(const std::wstring& name) {
			name_ = name;
		}

		// To comply with ComboBoxWnd::setStrings
		explicit operator std::wstring() const {
			return getName();
		}

	};

protected:
	int onSelChange(int index) {
		if(index == -1)
			onItemChange_(*this, T());
		else {
			auto i = selector_.begin();
			for(int k = 0; k < index; ++k, ++i);
			onItemChange_(*this, i->getData());
		}
		return _ComboBoxT::onSelChange(index);
	}

private:
	std::set<_Mapper> selector_;
	WndEventExtCaller<T> onItemChange_;
};

}

#endif /* COMBOBOX_H_ */
