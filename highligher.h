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
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat commentFormat;
    QTextCharFormat instructionFormat;
    QTextCharFormat registerFormat;
    QTextCharFormat labelFormat;
};

#endif // HIGHLIGHER_H
