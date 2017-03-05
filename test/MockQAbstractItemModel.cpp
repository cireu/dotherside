#include "MockQAbstractItemModel.h"
#include <QMetaType>

using namespace std;

namespace
{
    std::string toStringFromQVariant(const DosQVariant* variant) {
        CharPointer charArray(dos_qvariant_toString(variant), &dos_chararray_delete);
        return std::string(charArray.get());
    }

    VoidPointer initializeMetaObject()
    {
        void* superClassMetaObject = dos_qabstractitemmodel_qmetaobject();

        // Signals
        ::SignalDefinition signalDefinitionArray[1];

        signalDefinitionArray[0].name = "nameChanged";
        signalDefinitionArray[0].parametersCount = 1;
        int nameChanged[1];
        nameChanged[0] = QMetaType::QString;
        signalDefinitionArray[0].parametersMetaTypes = nameChanged;

        ::SignalDefinitions signalDefinitions;
        signalDefinitions.count = 1;
        signalDefinitions.definitions = signalDefinitionArray;

        // Slots
        ::SlotDefinition slotDefinitionArray[2];

        slotDefinitionArray[0].name = "name";
        slotDefinitionArray[0].returnMetaType = QMetaType::QString;
        slotDefinitionArray[0].parametersCount = 0;
        slotDefinitionArray[0].parametersMetaTypes = nullptr;

        slotDefinitionArray[1].name = "setName";
        slotDefinitionArray[1].returnMetaType = QMetaType::Void;
        int setNameParameters[1];
        setNameParameters[0] = QMetaType::QString;
        slotDefinitionArray[1].parametersCount = 1;
        slotDefinitionArray[1].parametersMetaTypes = setNameParameters;

        ::SlotDefinitions slotDefinitions;
        slotDefinitions.count = 2;
        slotDefinitions.definitions = slotDefinitionArray;

        // Properties
        ::PropertyDefinition propertyDefinitionArray[1];
        propertyDefinitionArray[0].name = "name";
        propertyDefinitionArray[0].notifySignal = "nameChanged";
        propertyDefinitionArray[0].propertyMetaType = QMetaType::QString;
        propertyDefinitionArray[0].readSlot = "name";
        propertyDefinitionArray[0].writeSlot = "setName";

        ::PropertyDefinitions propertyDefinitions;
        propertyDefinitions.count = 1;
        propertyDefinitions.definitions = propertyDefinitionArray;

        return VoidPointer(dos_qmetaobject_create(superClassMetaObject, "MockQAbstractListModel", &signalDefinitions, &slotDefinitions, &propertyDefinitions),
                           &dos_qmetaobject_delete);
    }
}

MockQAbstractItemModel::MockQAbstractItemModel()
    : m_vptr(dos_qabstractitemmodel_create(this, metaObject(), &onSlotCalled, &onRowCountCalled,
                                           &onColumnCountCalled, &onDataCalled, &onSetDataCalled,
                                           &onRoleNamesCalled, &onFlagsCalled, &onHeaderDataCalled), &dos_qobject_delete)
    , m_names({"John", "Mary", "Andy", "Anna"})
{

}

DosQMetaObject *MockQAbstractItemModel::metaObject()
{
    static VoidPointer result = initializeMetaObject();
    return result.get();
}

DosQObject *MockQAbstractItemModel::data()
{
    return m_vptr.get();
}

string MockQAbstractItemModel::objectName() const
{
    CharPointer result (dos_qobject_objectName(m_vptr.get()), &dos_chararray_delete);
    return string(result.get());
}

void MockQAbstractItemModel::setObjectName(const string &objectName)
{
    dos_qobject_setObjectName(m_vptr.get(), objectName.c_str());
}

string MockQAbstractItemModel::name() const
{
    return m_name;
}

void MockQAbstractItemModel::setName(const string &name)
{
    if (m_name == name)
        return;
    m_name = name;
    nameChanged(name);
}

void MockQAbstractItemModel::nameChanged(const string &name)
{
    int argc = 1;
    DosQVariant* argv[1];
    argv[0] = dos_qvariant_create_string(name.c_str());
    dos_qobject_signal_emit(m_vptr.get(), "nameChanged", argc, argv);
    dos_qvariant_delete(argv[0]);
}

void MockQAbstractItemModel::onSlotCalled(void *selfVPtr, DosQVariant *dosSlotNameVariant, int dosSlotArgc, DosQVariant **dosSlotArgv)
{
    auto self = static_cast<MockQAbstractItemModel*>(selfVPtr);
    string slotName = toStringFromQVariant(dosSlotNameVariant);

    if (slotName == "name") {
        VoidPointer name(dos_qvariant_create_string(self->name().c_str()), &dos_qvariant_delete);
        dos_qvariant_assign(dosSlotArgv[0], name.get());
        return;
    }

    if (slotName == "setName") {
        self->setName(toStringFromQVariant(dosSlotArgv[1]));
        return;
    }
}

void MockQAbstractItemModel::onRowCountCalled(void *selfVPtr, const DosQModelIndex *index, int *result)
{
    auto self = static_cast<MockQAbstractItemModel*>(selfVPtr);
    *result = self->m_names.size();
}

void MockQAbstractItemModel::onColumnCountCalled(void *selfVPtr, const DosQModelIndex *index, int *result)
{
    auto self = static_cast<MockQAbstractItemModel*>(selfVPtr);
    *result = 1;
}

void MockQAbstractItemModel::onDataCalled(void *selfVPtr, const DosQModelIndex *index, int role, DosQVariant *result)
{
    auto self = static_cast<MockQAbstractItemModel*>(selfVPtr);

    if (!dos_qmodelindex_isValid(index))
        return;

    const int row = dos_qmodelindex_row(index);
    const int column = dos_qmodelindex_column(index);

    if (row < 0 || static_cast<size_t>(row) >= self->m_names.size() || column != 0)
        return;

    dos_qvariant_setString(result, self->m_names[row].c_str());
}

void MockQAbstractItemModel::onSetDataCalled(void *selfVPtr, const DosQModelIndex *index, const DosQVariant *value, int role, bool *result)
{
    auto self = static_cast<MockQAbstractItemModel*>(selfVPtr);
    *result = false;

    if (!dos_qmodelindex_isValid(index))
        return;

    const int row = dos_qmodelindex_row(index);
    const int column = dos_qmodelindex_column(index);

    if (row < 0 || static_cast<size_t>(row) >= self->m_names.size() || column != 0)
        return;

    self->m_names[row] = toStringFromQVariant(value);
    dos_qabstractitemmodel_dataChanged(self->data(), index, index, 0, 0);

    *result = true;
}

void MockQAbstractItemModel::onRoleNamesCalled(void *selfVPtr, DosQHashIntQByteArray *result)
{

}

void MockQAbstractItemModel::onFlagsCalled(void *selfVPtr, const DosQModelIndex *index, int *result)
{

}

void MockQAbstractItemModel::onHeaderDataCalled(void *selfVPtr, int section, int orientation, int role, DosQVariant *result)
{

}
