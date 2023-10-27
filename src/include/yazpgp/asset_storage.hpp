#pragma once
#include <string>
#include <map>
#include <memory>
#include "logger.hpp"

namespace yazpgp
{
    template<typename T>
    class AssetStorage
    {
        std::map<std::string, std::shared_ptr<T>> m_assets;
    public:

        /**
         * @brief returns a shared pointer to the asset with the given name
         * 
         * @param name 
         * @return std::shared_ptr<T> nullptr if not found 
         */
        std::shared_ptr<T> get(const std::string& name) const
        {
            auto it = m_assets.find(name);
            if (it == m_assets.end())
            {
                YAZPGP_LOG_WARN("AssetStorage::get: asset \"%s\" not found", name.c_str());
                return nullptr;
            }
            return it->second;
        }


        std::shared_ptr<T> operator[](const std::string& name) const
        {
            return get(name);
        }

        /**
         * @brief adds an asset to the manager
         * 
         * @note if an asset with the same name already exists, it will be overwritten
         * 
         * @param name 
         * @param asset 
         * @return true if added successfully 
         * @return false if asset pointer is null
         */
        bool add(const std::string& name, std::shared_ptr<T> asset)
        {
            if (not asset)
            {
                YAZPGP_LOG_WARN("AssetStorage::add: asset pointer for \"%s\" is null", name.c_str());
                return false;
            }

            m_assets[name] = asset;
            return true;
        }

        /**
         * @brief removes an asset from the manager
         * 
         * @param name 
         * @return true if removed successfully 
         * @return false if asset with the given name does not exist
         */
        bool remove(const std::string& name)
        {
            auto it = m_assets.find(name);
            if (it == m_assets.end())
            {
                YAZPGP_LOG_WARN("AssetStorage::remove: asset \"%s\" not found", name.c_str());
                return false;
            }
            m_assets.erase(it);
            return true;
        }
    };
}