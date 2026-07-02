#ifndef AGENDAVISITOR_H
#define AGENDAVISITOR_H

class Appuntamento;
class Attivita;
class Consegna;
class Evento;
class Festivita;

// modifiche seconda consegna: visitor per gestire comportamenti specifici dei tipi Agenda.
class AgendaVisitor
{
public:
    virtual ~AgendaVisitor() = default;

    virtual void visit(const Attivita &attivita) = 0;
    virtual void visit(const Evento &evento) = 0;
    virtual void visit(const Appuntamento &appuntamento) = 0;
    virtual void visit(const Consegna &consegna) = 0;
    virtual void visit(const Festivita &festivita) = 0;
};

#endif
