#include "functionsoftransformingcpp.h"




QDataStream &operator>>(QDataStream &in_stream, Vechs &data)
{
    quint32 tempId;
    in_stream >> tempId // Считываем ID во временную переменную, обходя const
              >> data.m_vechsSeqNum
              >> data.m_isValid
              >> data.m_azimuth
              >> data.m_vert_angle
              >> data.m_distance;
    return in_stream;
}


QDataStream &operator>>(QDataStream &in_stream, Control &data)
{
    quint32 tempId;
    in_stream >> tempId
              >> data.m_controlSeqNum
              >> data.m_speed_regime
              >> data.m_speed_value
              >> data.m_auto_cmd_following;
    return in_stream;
}

QDataStream &operator>>(QDataStream &in_stream, cmdFlip &data)
{
    quint32 tempId;
    in_stream >> tempId
              >> data.cmdSeqNum
              >> data.azimuth
              >> data.elevation;
    return in_stream;
}



QDataStream &operator<<(QDataStream &out_stream, const TheodoliteInstance &data) {

    out_stream << data.idTheodoliteInstance
               << data.m_packetSeqNum
               << data.m_timestamp
               << data.status
               << data.m_mode
               << data.m_teodolite_turned_on
               << data.m_auto_following
               << data.m_distance;

    // Безопасно записываем сырой массив символов (64 байта)
    out_stream.writeRawData(data.m_status_msg, sizeof(data.m_status_msg));

    return out_stream;
}


QDataStream &operator<<(QDataStream &out_stream, const TheodoliteCmd& theodoliteDate)
{
    out_stream << static_cast<quint32>(theodoliteDate.index()); //Заставляем поток считывать указанный тип по индексу

    std::visit([&out_stream](const auto &arg) {
        out_stream << arg; // Вызывается QDataStream::operator<<(const T&)
    }, theodoliteDate);

    return out_stream;
}
