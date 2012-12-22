/*
 * UndoMaintainer.h
 *
 *  Created on: 21.12.2012
 *      Author: Morphe
 */

#ifndef UNDOMAINTAINER_H_
#define UNDOMAINTAINER_H_
#include <stack>
#include <memory>

namespace XEditor {

class XEditorWindow;
class UndoOperation;

class UndoMaintainer {
public:
	UndoMaintainer(XEditorWindow &editor);
	~UndoMaintainer();

	void undo();
	void redo();

	void accumulateRemove(int line, int pos, wchar_t ch, bool back);
	void accumulateInput(int line, int pos, bool lineFeed);
	void storeRemove(int line, int pos, const std::wstring &text);
	void storeInput(int line, int pos, int endLine, int endPos);
	void storeReplace(int line, int pos, int endLine, int endPos, const std::wstring &text);
	void resetRedo();
	void storeAccumulator();

private:
	XEditorWindow &back_ref_;
	std::stack<std::unique_ptr<UndoOperation>> undoStack_, redoStack_;
	std::unique_ptr<UndoOperation> accumulator_;
};

} /* namespace XEditor */
#endif /* UNDOMAINTAINER_H_ */
