/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/memo.hpp>

namespace graphene { namespace chain {

        /**
         * @ingroup operations
         *
         * @brief Transfers an amount of one asset from one account to another
         *
         *  Fees are paid by the "from" account
         *
         *  @pre amount.amount > 0
         *  @pre fee.amount >= 0
         *  @pre from != to
         *  @post from account's balance will be reduced by fee and amount
         *  @post to account's balance will be increased by amount
         *  @return n/a
         */
        struct keyword_contract_operation : public base_operation
        {
            // TODO: Change fee parameters
            struct fee_parameters_type {
                uint64_t fee       = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
                uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
            };

            asset            fee;

            account_id_type  from;

            account_id_type  to;

            asset            amount;

            optional<memo_data> memo;
            extensions_type   extensions;

            account_id_type fee_payer()const { return from; }
            void            validate()const;
            share_type      calculate_fee(const fee_parameters_type& k)const { return 0; }
        };

    }} // graphene::chain

FC_REFLECT( graphene::chain::keyword_contract_operation::fee_parameters_type, (fee)(price_per_kbyte) )

FC_REFLECT( graphene::chain::keyword_contract_operation, (fee)(from)(to)(amount)(memo)(extensions) )