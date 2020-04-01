#pragma once

#include <QtGui/QSyntaxHighlighter>

class SyntaxHighlighter: public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SyntaxHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text) override;

private:
    QRegularExpression statementExp;
    QTextCharFormat formatPunctuation;
    QTextCharFormat formatText;
    QTextCharFormat formatColour;
};
