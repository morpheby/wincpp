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

namespace ComboBox {

template <class T>
struct _ConstNamer {
	std::wstring name_;
	_ConstNamer(const std::wstring& name) : name_(name) {};

	std::wstring operator() (const T& notused) const { return name_; }
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
		resetList();
	}

	void addItem(const T& item) {
		dataContainer.push_back(_Mapper(item));
		resetList();
	}

	void resetList() {
		sort(dataContainer.begin(), dataContainer.end());
		this->setStrings(dataContainer);
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

		wstring getName() const {
			return namer_(data_);
		}

		// To comply with ComboBoxWnd::setStrings
		explicit operator wstring() const {
			return getName();
		}

	};

protected:
	int onSelChange(int index) {
		if(index == -1)
			onItemChange_(*this, T());
		else
			onItemChange_(*this, dataContainer[index].getData());
		return _ComboBoxT::onSelChange(index);
	}

private:
	vector<_Mapper> dataContainer;
	WndEventExtCaller<T> onItemChange_;
};

/*
template <class _ComboBoxT, typename T,
	class _Cmp = std::less<T>, class _Eq = std::equal_to<T>>
class Selector<_ComboBoxT, T, _ConstNamer<T>, _Cmp, _Eq>  : public _ComboBoxT {
public:
	Selector(int x, int y, int width, int height, Window& parent) :
		_ComboBoxT(x, y, width, height, parent) {
	}
	~Selector() {}

	void addItems(initializer_list<pair<_Namer, T>> l) {
		for(auto& i : l)
			dataContainer.insert(i);
		this->setStrings(dataContainer);
	}

	template<class _ContainerT, class _Namer>
	void setItems(_ContainerT container, _Namer namer) {
		dataContainer.clear();
		for(auto& i : container)
			dataContainer.push_back(_Mapper(namer, i));
		resetList();
	}

	void addItem(const wstring& name, const shared_ptr<T>& item) {
		dataContainer.push_back(_Mapper(_ConstNamer<T>(name), item));
		this->setStrings(dataContainer);
	}

	void removeItem(const wstring& name) {
		dataContainer.remove(_Mapper(name, T()));
		this->setStrings(dataContainer);
	}

	void resetList() {
		set<_Mapper> t(dataContainer.begin(), dataContainer.end());
		this->setStrings(t);
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

		_Mapper(_Mapper&& m) :
			namer_(m.namer_), data_(m.data_) {
			m.namer_ = 0;
		}

		_Mapper(const pair<_Namer, T>& c) :
			namer_(c.first), data_(c.second){
		}

		_Mapper(const _Namer& name) : namer_(name) {
		}

		template<class _Namer>
		_Mapper(_Namer namer, const T& data) : namer_(namer),
			data_(data) {
		}

		~_Mapper() {
			delete namer_;
		}

		bool operator<(const _Mapper& m) const {
			return cmp(data_, m.data_);
		}

		bool operator==(const _Mapper& m) const {
			return  eq(data_, m.data_);
		}

		T getData() const {
			return data_;
		}

		wstring getName() const {
			return namer_(data_);
		}

		// To comply with ComboBoxWnd::setStrings
		explicit operator wstring() const {
			return getName();
		}

	};

protected:
	int onSelChange(const wstring& name) {
		if(name.empty())
			onItemChange_(*this, 0);
		else {
			auto i = dataMap.find(name);
			if(i == dataMap.end())
				RebuildMap();
			onItemChange_(*this, dataMap[name]);
		}
		return _ComboBoxT::onSelChange(name);
	}

private:
	set<_Mapper> dataContainer;
	unordered_map<wstring, T> dataMap;
	WndEventExtCaller<T> onItemChange_;

	void RebuildMap() {
		dataMap.clear();
		for(auto& i : dataContainer)
			dataMap[(wstring)i] = i.getData();
	}
};*/

}

#endif /* COMBOBOX_H_ */
