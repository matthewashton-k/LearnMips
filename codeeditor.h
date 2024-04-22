/**
 * Description: header file for the Code Editor class, which represents a widget used to edit Mips Code with syntax highlighting and line numbers
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    /**
     * @brief CodeEditor constructor for the code editor
     * @param parent parent widget
     */
    CodeEditor(QWidget *parent = nullptr);

    /**
     * @brief lineNumberAreaPaintEvent catches line number paint events
     * @param event the event
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    /**
     * @brief lineNumberAreaWidth
     * @return
     */
    int lineNumberAreaWidth();

protected:
    /**
     * @brief resizeEvent override the resize event
     * @param event the event
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief updateLineNumberAreaWidth allows the width of the margin to be modified
     * @param newBlockCount the new size
     */
    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * @brief highlightCurrentLine highlights the current line
     */
    void highlightCurrentLine();

    /**
     * @brief updateLineNumberArea updates the margin size
     * @param rect rect to follow size of
     * @param dy change in height
     */
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    /**
     * @brief lineNumberArea pointer to the line number widget
     */
    QWidget *lineNumberArea;
};

/**
 * @brief The LineNumberArea class a helper class representing the line numbers of the code editor
 */
class LineNumberArea : public QWidget
{
public:
    /**
     * @brief LineNumberArea constructor for the line numbers in the margin
     * @param editor parent editor
     */
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {}

    /**
     * @brief sizeHint getter for the size hint for the line numbers
     * @return  the size hint
     */
    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    /**
     * @brief paintEvent paint event override
     * @param event the event
     */
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:

    // pointer to the parent code editor
    CodeEditor *codeEditor;
};
#endif // CODEEDITOR_H
