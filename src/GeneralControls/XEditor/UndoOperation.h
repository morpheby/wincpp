/*
 * UndoOperation.h
 *
 *  Created on: 21.12.2012
 *      Author: Morphe
 */

#ifndef UNDOOPERATION_H_
#define UNDOOPERATION_H_

#include <memory>
#include <string>
#include <utility>

namespace XEditor {

class XEditorWindow;

class UndoOperation {
public:
	UndoOperation(int undoCursorLine, int undoCursorPos);
	virtual ~UndoOperation();

	virtual std::unique_ptr<UndoOperation> undo(XEditorWindow &editor);

	int getLine() const {
		return line_;
	}

	int getPos() const {
		return pos_;
	}

private:
	const int line_, pos_;
};

class InsertOperation : public UndoOperation {
public:
	InsertOperation(int line, int position, const std::wstring &text);
	~InsertOperation();

	std::unique_ptr<UndoOperation> undo(XEditorWindow &editor) override;

	const std::wstring& getText() const {
		return insertText_;
	}

private:
	const std::wstring insertText_;
};

class RemoveOperation : public UndoOperation {
public:
	RemoveOperation(int line, int position, int endLine, int endPosition);
	~RemoveOperation();

	std::unique_ptr<UndoOperation> undo(XEditorWindow &editor) override;

	const int getEndLine() const {
		return endLine_;
	}

	const int getEndPos() const {
		return endPos_;
	}

private:
	const int endLine_, endPos_;
};

class ReplaceOperation : public UndoOperation {
public:
	ReplaceOperation(int line, int position, int endLine, int endPosition, const std::wstring &text);
	~ReplaceOperation();

	std::unique_ptr<UndoOperation> undo(XEditorWindow &editor) override;

	const int getEndLine() const {
		return endLine_;
	}

	const int getEndPos() const {
		return endPos_;
	}

	const std::wstring& getReplaceText() const {
		return replaceText_;
	}

private:
	const int endLine_, endPos_;
	const std::wstring replaceText_;
};

} /* namespace XEditor */
#endif /* UNDOOPERATION_H_ */
