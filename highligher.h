/**
 * Description: header file for the Highlighter class, which is a custom syntax highlighter that extends QSyntaxHighlighter
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/
#ifndef HIGHLIGHER_H
#define HIGHLIGHER_H

#include <QWidget>
#include <QSyntaxHighlighter>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    /**
     * @brief Highlighter constructor for highlighter
     * @param parent
     */
    Highlighter(QTextDocument *parent = 0);

protected:
    /**
     * @brief highlightBlock override of highlight block method
     * @param text text to highlight
     */
    void highlightBlock(const QString &text) override;

private:
    // struct for highlighting rules
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    //format stuff
    QTextCharFormat commentFormat;
    QTextCharFormat instructionFormat;
    QTextCharFormat registerFormat;
    QTextCharFormat labelFormat;
};

#endif // HIGHLIGHER_H
