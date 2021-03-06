#pragma once
#include "filesystem_resolver.hpp"



namespace args::core::filesystem
{
    class ARGS_API mem_filesystem_resolver : public filesystem_resolver, public memory_resolver_common_base
    {
    public:
        /** Requires the raw data to be passed for internal building.
         */
        explicit mem_filesystem_resolver(std::shared_ptr<const byte_vec> target_data);

        /** @brief Makes sure that the internal fast memory representation has been built
         *  when false is returned it was unable to build it.
         */
        bool prewarm() const;

        /** @brief Sets the raw data of the provider.
         */
        void set_disk_data(const byte_vec& target_data)
        {
            m_targetData = std::make_shared<const byte_vec>(target_data.begin(), target_data.end());
        }
        /** @brief Sets the raw data of the provider.
         */
        void set_disk_data(byte_vec&& target_data)
        {
            m_targetData = std::make_shared<const byte_vec>(std::move(target_data));
        }
        /** @brief Sets the raw data of the provider.
         */
        void set_disk_data(std::shared_ptr<const byte_vec> target_data)
        {
            m_targetData = target_data;
        }

        A_NODISCARD filesystem_resolver* make() final override
        {
            mem_filesystem_resolver* x = make_higher();
            x->set_disk_data(m_targetData);
            return x;
        }

        /** @brief Same as filesystem_provider::make, override this instead of make
         *         for all mem_filesystem_resolvers.
         *
         *  @ref filesystem_provider::make
         */
        A_NODISCARD virtual mem_filesystem_resolver* make_higher() ARGS_PURE;


    protected:

        /**@brief Returns the fast memory representation of the provider.
         *        Created by build_memory_representation.
         */
        A_NODISCARD const byte_vec& get_data() const;
        A_NODISCARD byte_vec& get_data();


        /**@brief Should build a fast memory representation of the virtual filesystem.
         *        For instance, when inflating a zip archive this should put the
         *        inflated memory into out.
         *
         * @param [in] in The array with compressed data.
         * @param [out] out The array where to put uncompressed data.
         */
        virtual void build_memory_representation(std::shared_ptr<const byte_vec> in, std::shared_ptr<byte_vec> out) const ARGS_PURE;

        /**@brief Should build the representation on how the data is saved to disk,
         *        when the filesystem is readonly this does not apply.
         *
         * @param [in] in The array with decompressed  data.
         * @param [out] out The array where to put compressed data.
         */
        virtual void build_disk_representation(std::shared_ptr<const byte_vec> in, std::shared_ptr<byte_vec> out) const ARGS_IMPURE;

        /**@brief A hint to how big the fast memory representation is going to be.
         * @param [in] in The array with the compressed data.
         */
        virtual std::size_t size_hint(std::shared_ptr<const byte_vec> in) const ARGS_IMPURE_RETURN(0)


    private:
        mutable std::shared_ptr<byte_vec> m_data;
        std::shared_ptr<const byte_vec> m_targetData;
    };
}
