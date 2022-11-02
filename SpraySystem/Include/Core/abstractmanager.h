#ifndef ABSTRACTDEVICEMANAGER_H
#define ABSTRACTDEVICEMANAGER_H
#include "core/properties/property.hpp"
#include "core/config/config.hpp"
#include "core/visibility_control.hpp"
#include "core/plugin/vwfactorymanager.h"
#include "abstractmodel.h"
#include "abstractmanagerbase.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <QObject>
#include <QUuid>
#include <mutex>
namespace vws {
namespace core {
using Property = vws::properties::Property;
using Config = vws::properties::Config;
using factory = vws::factory::vwFactoryManager;


// class CORE_MODULE AbstractManagerBase : public QObject{
//     Q_OBJECT
// public:
//     AbstractManagerBase(){}
// public slots:
//     virtual void onModelNameChanged(std::string oldName, std::string newName){
//     }

// };



template<typename T>
class AbstractManager : public AbstractManagerBase
{
public:
   // static std::shared_ptr<T> invalidPtr;

    AbstractManager(){}
    virtual ~AbstractManager(){}

    // 创建设备后，将设备加入Map中，
    ///
    /// \brief create
    /// 创建一个设备，并注册到管理器中。
    /// 设备的名字为派生类的名字
    /// \param derived
    /// \return
    ///
    std::shared_ptr<T> create(std::string derived){
        
        

        auto p = factory::getManager()->create<T>(derived);
        if (p != nullptr){
            std::string defaultName = p->defaultName();
            std::string name = formIndexedDefaultName(defaultName);
            while(find(name)!= nullptr){
                name = formIndexedDefaultName(defaultName);
            }

            p->setModelNameSpace(defaultNameSpace());
            p->setModelName(name);
            p->setManager(this);
            
            // connect(p.get(), &AbstractModel::requestChangeModelName, this, &AbstractManager::onModelNameChanged);
            //connect(p.get(), SIGNAL(requestChangeModelName(std::string, std::string)), this, SLOT(onModelNameChanged(std::string, std::string)));
            nameMap_[name] = p;
            idMap_[p->id().toString().toStdString()] = p;
        }
        return p;
    }

    std::string formIndexedDefaultName(std::string name) {
        static uint64_t id = 1;
        std::mutex m;
        std::stringstream ss;
        std::lock_guard<std::mutex> lock(m);
        ss << name << " " << id++;
        return ss.str();

    }

    virtual std::string defaultNameSpace() const {
        return "global";
    }

    std::shared_ptr<T> find(const QUuid &id){
        std::string idstr = id.toString().toStdString();
        if (idMap_.find(idstr) != idMap_.end()){
            return idMap_[idstr];
        }else{
            return nullptr;
        }
    }

    std::shared_ptr<T> find(const std::string &name){
        if (nameMap_.find(name) != nameMap_.end()){
            return nameMap_[name];
        }else{
            return nullptr;
        }
    }

    void remove(const QUuid &id){
        std::string idstr = id.toString().toStdString();
        if(idMap_.find(idstr) != idMap_.end()){
            idMap_.erase(idstr);
        }
    }

    virtual void save(Config c) const{
        auto viewList = c.mapMakeChild(QString::fromStdString(defaultNameSpace()));
        for (auto it = idMap_.begin(); it != idMap_.end(); ++it) {
            auto pcam = it->second;
            std::string className = pcam->getClassName();
            auto node = viewList.listAppendNew();
            pcam->save(node);
        }
    }

    virtual void load(const Config& c){
        auto viewList = c.mapGetChild(QString::fromStdString(defaultNameSpace()));
        int N = viewList.listLength();
        for (int i = 0; i < N; ++i) {
            Config c = viewList.listChildAt(i);
            QString derived;
            c.mapGetString("class", &derived);
            auto pcam = create(derived.toStdString());
            pcam->load(c);
        }
    }

    virtual bool ChangeModelName(std::string oldName, std::string newName) override{
        // 如果新名字和旧名字一致
        if(!oldName.compare(newName)){
            return true;
        }
        auto it_old = nameMap_.find(oldName);
        // 如果新名字和其他model产生冲突
        auto it = nameMap_.find(newName);
        if ( it != nameMap_.end()
                && it->second->id() != it_old->second->id()){
            std::cout << "Name Conflict" << std::endl;
            return false;
        }
        if (nameMap_.find(oldName) != nameMap_.end()){
            auto p = nameMap_[oldName];

            nameMap_.erase(oldName);
            p->setModelName(newName);
            nameMap_[newName] = p;
            return true;
        }
        return false;
    }
    // virtual void onModelNameChanged(std::string oldName, std::string newName) override{
    //     // 如果新名字和旧名字一致
    //     if(!oldName.compare(newName)){
    //         return;
    //     }
    //     auto it_old = nameMap_.find(oldName);
    //     // 如果新名字和其他model产生冲突
    //     auto it = nameMap_.find(newName);
    //     if ( it != nameMap_.end()
    //             && it->second->id() != it_old->second->id()){
    //         std::cout << "Name Conflict" << std::endl;
    //         return;
    //     }
    //     if (nameMap_.find(oldName) != nameMap_.end()){
    //         auto p = nameMap_[oldName];

    //         nameMap_.erase(oldName);
    //         p->setModelName(newName);
    //         nameMap_[newName] = p;
    //     }
    // }

protected:
    std::unordered_map<std::string, std::shared_ptr<T>> idMap_;
    std::unordered_map<std::string, std::shared_ptr<T>> nameMap_;
};
}
}

#endif // ABSTRACTDEVICEMANAGER_H



















