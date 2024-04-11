#include "highligher.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Comment format (e.g., starting with '#')
    commentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = commentFormat;
    highlightingRules.append(rule);

    // Instruction format (e.g., ADD, SUB, LW, SW, etc.)
    instructionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b(ADD|SUB|LW|SW|J|JR|BEQ)\\b"));
    rule.format = instructionFormat;
    highlightingRules.append(rule);

    // Register format (e.g., $s0, $t1, $a2, etc.)
    registerFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("\\$[a-zA-Z0-9]+"));
    rule.format = registerFormat;
    highlightingRules.append(rule);

    // Label format (e.g., loop:, start:, etc.)
    labelFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[a-zA-Z_][a-zA-Z0-9_]*:"));
    rule.format = labelFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
