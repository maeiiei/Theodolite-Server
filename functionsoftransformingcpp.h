#ifndef FUNCTIONSOFTRANSFORMING_H
#define FUNCTIONSOFTRANSFORMING_H

#include <QDataStream>
#include "../theodolite_data/mydata.h"
#include <variant>

typedef  std::variant<Vechs, Control, cmdFlip> TheodoliteCmd;

QDataStream & operator>>(QDataStream &in_stream, Vechs & data); //Чтение из потока

QDataStream &operator>>(QDataStream &in_stream, Control & data); //Чтение

QDataStream &operator>>(QDataStream &in_stream, cmdFlip & data); //Чтение

QDataStream &operator<<(QDataStream &out_stream, const TheodoliteInstance &data); //Запись в поток

QDataStream &operator<<(QDataStream &out_stream, const TheodoliteCmd& theodoliteDate);
#endif // FUNCTIONSOFTRANSFORMING_H
