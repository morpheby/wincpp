/*
 * UndoMaintainer.cpp
 *
 *  Created on: 21.12.2012
 *      Author: Morphe
 */

#include "UndoMaintainer.h"
#include "UndoOperation.h"

namespace XEditor {

UndoMaintainer::UndoMaintainer(XEditorWindow& editor) : back_ref_(editor) {
}

UndoMaintainer::~UndoMaintainer() {
}

void UndoMaintainer::undo() {
	if(accumulator_) {
		redoStack_.push(accumulator_->undo(back_ref_));
		accumulator_.reset(nullptr);
	} else if(!undoStack_.empty()) {
		redoStack_.push(undoStack_.top()->undo(back_ref_));
		undoStack_.pop();
	}
}

void UndoMaintainer::redo() {
	storeAccumulator();
	if(!redoStack_.empty()) {
		undoStack_.push(redoStack_.top()->undo(back_ref_));
		redoStack_.pop();
	}
}

void UndoMaintainer::accumulateRemove(int line, int pos, wchar_t ch, bool back) {
	resetRedo();
	if(InsertOperation *input = dynamic_cast<InsertOperation*>(accumulator_.get()))
		if(back)
			accumulator_ = std::unique_ptr<UndoOperation> (new InsertOperation(line, pos, ch + input->getText()));
		else
			accumulator_ = std::unique_ptr<UndoOperation> (new InsertOperation(line, pos, input->getText() + ch));
	else
		accumulator_ = std::unique_ptr<UndoOperation> (new InsertOperation(line, pos, std::wstring(1, ch)));
}

void UndoMaintainer::accumulateInput(int line, int pos, bool lineFeed) {
	resetRedo();
	if(RemoveOperation *remove = dynamic_cast<RemoveOperation*>(accumulator_.get()))
		accumulator_ = std::unique_ptr<UndoOperation> (new RemoveOperation(remove->getLine(), remove->getPos(), lineFeed ? line + 1 : line, lineFeed ? 0 : pos + 1));
	else
		accumulator_ = std::unique_ptr<UndoOperation> (new RemoveOperation(line, pos, lineFeed ? line + 1 : line, lineFeed ? 0 : pos + 1));
}

void UndoMaintainer::storeRemove(int line, int pos, const std::wstring& text) {
	resetRedo();
	storeAccumulator();
	undoStack_.push(std::unique_ptr<UndoOperation> (new InsertOperation(line, pos, text)));
}

void UndoMaintainer::storeInput(int line, int pos, int endLine, int endPos) {
	resetRedo();
	storeAccumulator();
	undoStack_.push(std::unique_ptr<UndoOperation> (new RemoveOperation(line, pos, endLine, endPos)));
}

void UndoMaintainer::storeReplace(int line, int pos, int endLine, int endPos,
		const std::wstring& text) {
	resetRedo();
	storeAccumulator();
	undoStack_.push(std::unique_ptr<UndoOperation> (new ReplaceOperation(line, pos, endLine, endPos, text)));
}

void UndoMaintainer::storeAccumulator() {
	if (accumulator_)
		undoStack_.push(std::unique_ptr<UndoOperation>(accumulator_.release()));
}

void UndoMaintainer::resetRedo() {
	while(!redoStack_.empty()) redoStack_.pop();
}

} /* namespace XEditor */
