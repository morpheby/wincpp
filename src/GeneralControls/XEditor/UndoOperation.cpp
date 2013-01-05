/*
 * UndoOperation.cpp
 *
 *  Created on: 21.12.2012
 *      Author: Morphe
 */

#include "UndoOperation.h"
#include "XEditorWindow.h"
#include <algorithm>

namespace XEditor {

UndoOperation::UndoOperation(int undoCursorLine, int undoCursorPos) :
	line_{undoCursorLine}, pos_{undoCursorPos} {
}

UndoOperation::~UndoOperation() {
}

std::unique_ptr<UndoOperation> UndoOperation::undo(XEditorWindow& editor) {
	editor.setCurrentPosition(line_, pos_);
	return std::unique_ptr<UndoOperation> (new UndoOperation(line_, pos_));
}

InsertOperation::InsertOperation(int line, int position,
		const std::wstring& text) :
			UndoOperation(line, position),
			insertText_{text} {
}

InsertOperation::~InsertOperation() {
}

std::unique_ptr<UndoOperation> InsertOperation::undo(XEditorWindow& editor) {
	editor.insertText(getLine(), getPos(), insertText_);

	int endLine = getLine() + std::count(insertText_.begin(), insertText_.end(), L'\n'),
		endPos = (endLine == getLine()) ? getPos() + insertText_.size() : insertText_.rfind(L'\n');

	editor.setSelection(getLine(), getPos(), endLine, endPos);

	return std::unique_ptr<UndoOperation> (new RemoveOperation(getLine(), getPos(), endLine, endPos));
}

RemoveOperation::RemoveOperation(int line, int position, int endLine,
		int endPosition) :
			UndoOperation(line, position), endLine_{endLine}, endPos_{endPosition} {

}

RemoveOperation::~RemoveOperation() {
}

std::unique_ptr<UndoOperation> RemoveOperation::undo(XEditorWindow& editor) {
	editor.setSelection(getLine(), getPos(), endLine_, endPos_);

	std::wstring text = editor.getSelection();
	editor.removeSelection();

	return std::unique_ptr<UndoOperation> (new InsertOperation(editor.getCurrentLinePosition(), editor.getCurrentCharPosition(), text));
}

ReplaceOperation::ReplaceOperation(int line, int position, int endLine,
		int endPosition, const std::wstring &text) :
			UndoOperation(line, position), endLine_{endLine}, endPos_{endPosition}, replaceText_{text} {
}

ReplaceOperation::~ReplaceOperation() {
}

std::unique_ptr<UndoOperation> ReplaceOperation::undo(
		XEditorWindow& editor) {
	editor.setSelection(getLine(), getPos(), endLine_, endPos_);

	std::wstring text = editor.getSelection();
	editor.removeSelection();

	int line = editor.getCurrentLinePosition(), pos = editor.getCurrentCharPosition();
	editor.insertText(line, pos, replaceText_);

	int endLine = line + std::count(replaceText_.begin(), replaceText_.end(), L'\n');
	int endPos = (endLine == line) ? pos + replaceText_.size() : replaceText_.rfind(L'\n');

	editor.setSelection(line, pos, endLine, endPos);

	return std::unique_ptr<UndoOperation> (new ReplaceOperation(line, pos, endLine, endPos, text));
}

} /* namespace XEditor */


