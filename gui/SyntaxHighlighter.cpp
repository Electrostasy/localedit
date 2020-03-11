#include <QtCore/QRegularExpression>
#include <QtCore/QLoggingCategory>
#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
    // TODO room for improvement, optimize later
    statementExp.setPattern(R"(<(Col)?(:?)((?<=(Col:))(?<colour>\s?\w+)?(?=(>?)))?>?(?<text>.*?(?=<))?(<?/(Col)?>)?)");
    formatColour.setForeground(QColor(2, 44, 252));
    formatColour.setFontWeight(QFont::Bold);
    formatText.setForeground(QColor(2, 169, 252));
    formatPunctuation.setForeground(QColor(0xfc, 0x7c, 0x05));
    formatPunctuation.setFontWeight(QFont::Bold);
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    QRegularExpressionMatchIterator it = statementExp.globalMatch(text);
    while(it.hasNext()) {
        QRegularExpressionMatch match = it.next();

        setFormat(match.capturedStart(), match.capturedLength(), formatPunctuation);
        setFormat(match.capturedStart("colour"), match.capturedLength("colour"), formatColour);
        setFormat(match.capturedStart("text"), match.capturedLength("text"), formatText);
    }
}

