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
#include <graphene/chain/keyword_contract_evaluator.hpp>
#include <graphene/chain/account_object.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/is_authorized_asset.hpp>
#include <websocketpp/common/md5.hpp>
#include <graphene/app/database_api.hpp>
#include <boost/algorithm/string.hpp>


namespace graphene { namespace chain {
        void_result keyword_contract_evaluator::do_evaluate( const keyword_contract_operation& op )
        { try {

                database& d = db();

                const account_object& from_account    = op.from(d);
                const account_object& to_account      = op.to(d);
                const asset_object&   asset_type      = op.amount.asset_id(d);

                graphene::app::database_api *api = new graphene::app::database_api(d);

                try {

                    GRAPHENE_ASSERT(
                            is_authorized_asset( d, from_account, asset_type ),
                            transfer_from_account_not_whitelisted,
                            "'from' account ${from} is not whitelisted for asset ${asset}",
                            ("from",op.from)
                                    ("asset",op.amount.asset_id)
                    );
                    GRAPHENE_ASSERT(
                            is_authorized_asset( d, to_account, asset_type ),
                            transfer_to_account_not_whitelisted,
                            "'to' account ${to} is not whitelisted for asset ${asset}",
                            ("to",op.to)
                                    ("asset",op.amount.asset_id)
                    );

                    if( asset_type.is_transfer_restricted() )
                    {
                        GRAPHENE_ASSERT(
                                from_account.id == asset_type.issuer || to_account.id == asset_type.issuer,
                                transfer_restricted_transfer_asset,
                                "Asset {asset} has transfer_restricted flag enabled",
                                ("asset", op.amount.asset_id)
                        );
                    }

                    bool insufficient_balance = d.get_balance( from_account, asset_type ).amount >= op.amount.amount;
                    FC_ASSERT( insufficient_balance,
                               "Insufficient Balance: ${balance}, unable to transfer '${total_transfer}' from account '${a}' to '${t}'",
                               ("a",from_account.name)("t",to_account.name)("total_transfer",d.to_pretty_string(op.amount))("balance",d.to_pretty_string(d.get_balance(from_account, asset_type))) );

                    return void_result();
                } FC_RETHROW_EXCEPTIONS( error, "Unable to transfer ${a} from ${f} to ${t}", ("a",d.to_pretty_string(op.amount))("f",op.from(d).name)("t",op.to(d).name) );

            }  FC_CAPTURE_AND_RETHROW( (op) ) }


        vector<string> keyword_contract_evaluator::split_by_delimeter(const string& str, const char& ch) {
            string next;
            vector<string> result;

            for (string::const_iterator it = str.begin(); it != str.end(); it++) {
                if (*it == ch) {
                    if (!next.empty()) {
                        result.push_back(next);
                        next.clear();
                    }
                } else {
                    next += *it;
                }
            }
            if (!next.empty())
                result.push_back(next);
            return result;
        }


        optional<asset_object> keyword_contract_evaluator::find_asset(string asset_symbol_or_id)const
        {
            FC_ASSERT( asset_symbol_or_id.size() > 0 );

            database& d = db();
            graphene::app::database_api *api = new graphene::app::database_api(d);
            ilog("Zashli v metod find_asset");
            auto rec = api->lookup_asset_symbols({asset_symbol_or_id}).front();
            ilog("Zashli v metod find_asset 2");
//            if( rec )
//            {
//                ilog("Zashli v metod find_asset 3");
//                if( rec->symbol != asset_symbol_or_id )
//                    ilog("Zashli v metod find_asset 4");
//                    return optional<asset_object>();
//            }
            ilog("Zashli v metod find_asset 5");
            return rec;
        }


        asset_object keyword_contract_evaluator::get_asset(string asset_symbol_or_id)const
        {
            ilog("Zashli v metod get_asset");
            auto opt = keyword_contract_evaluator::find_asset(asset_symbol_or_id);
            ilog("Zashli v metod get_asset 2");
            FC_ASSERT(opt);
            ilog("Zashli v metod get_asset 3");
            return *opt;
        }


        void_result keyword_contract_evaluator::do_apply( const keyword_contract_operation& o )
        { try {
                database& d = db();
                graphene::app::database_api *api = new graphene::app::database_api(d);
                const account_object& from_account    = o.from(d);
                const account_object& to_account      = o.to(d);

                // Check if it's keyword contract
                // TODO: To separate function

                FC_ASSERT (to_account.name.find("rgtkeyword") != std::string::npos);

                vector<string> hash = keyword_contract_evaluator::split_by_delimeter(to_account.name, '-');
                ilog("Zashli v metod do_apply");
                ilog(boost::to_upper_copy<std::string>(hash[3]));
                fc::optional<asset_object> asset_obj = get_asset(boost::to_upper_copy<std::string>(hash[3]));

                ilog("Ura");

                asset new_amount = asset_obj->amount_from_string(hash[2]);

                db().adjust_balance(o.to, -new_amount);
                db().adjust_balance(o.from, new_amount);
                db().action_keyword(o.to, o.from, new_amount);
                return void_result();
            } FC_CAPTURE_AND_RETHROW( (o) ) }

    } } // graphene::chain
