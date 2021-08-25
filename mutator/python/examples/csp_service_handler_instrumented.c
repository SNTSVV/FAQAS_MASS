/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/csp_service_handler.h>

//FAQAS header
#include <gs/param/FAQAS_dataDrivenMutator.h>

#include <gs/param/table.h>
#include <gs/param/store/store.h>
#include <gs/param/internal/rparam.h>
#include <gs/param/internal/serialize.h>
#include <gs/param/internal/types.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>
#include "host_log.h"
#include <stdio.h>
#include <param/rparam_client.h> // -> GS_RPARAM_LEGACY_MAGIC_CHECKSUM

static bool validate_checksum(gs_param_table_instance_t * tinst, uint16_t ref)
{
    if (tinst == NULL) {
        return false;
    }
    if (ref == gs_param_table_checksum(tinst)) {
        return true;
    }
    if (ref == gs_param_table_checksum_be(tinst)) {
        return true;
    }
    if ((ref == GS_RPARAM_MAGIC_CHECKSUM) || (ref == GS_RPARAM_LEGACY_MAGIC_CHECKSUM)) {
        return true;
    }
   return false;
}


//mutate function
void mutate(gs_rparam_query_t * request , FaultModel *fm ){
    

    v[0] = (unsigned long long int) request->action;
    v[1] = (unsigned long long int) request->table_id;
    v[2] = (unsigned long long int) request->length;
    v[3] = (unsigned long long int) request->checksum;
    v[4] = (unsigned long long int) request->seq;
    v[5] = (unsigned long long int) request->total;

    switch(request->action) {
        case RPARAM_GET:{
            v[6]= (unsigned long long int)request->payload.addr[0];
            v[7]=0;
        }

        case RPARAM_SET:{
            v[6]= (unsigned long long int)request->payload.packed[0];
            v[7]=0;
        }

        case RPARAM_SAVE:{
            v[6]= (unsigned long long int)request->payload.copy.from;
            v[7]= (unsigned long long int)request->payload.copy.to;
        }

        case RPARAM_LOAD:{
            v[6]= (unsigned long long int)request->payload.copy.from;
            v[7]= (unsigned long long int)request->payload.copy.to;
        }

        case RPARAM_COPY:{
            v[6]= (unsigned long long int)request->payload.copy.from;
            v[7]= (unsigned long long int)request->payload.copy.to;
        }

        case RPARAM_REPLY:{
            v[6]= (unsigned long long int)request->payload.packed[0];
        }   
    }


     _FAQAS_mutate(v,fm);

    request->action = (uint8_t) v[0];
    request->table_id = (uint8_t) v[1];
    request->length = (uint16_t) v[2];
    request->checksum = (uint16_t) v[3];
    request->seq = (uint16_t) v[4];
    request->total = (uint16_t) v[5];


    switch(request->action) {
        case RPARAM_GET:{
        
        request->payload.addr[0]=(uint16_t)v[6];
        
        }

        case RPARAM_SET:{
        
        request->payload.packed[0]=(uint8_t)v[6]; 

        }

        case RPARAM_SAVE:{
        
        request->payload.copy.from=(uint8_t)v[6];
        request->payload.copy.to=(uint8_t)v[7];       
        
        }

        case RPARAM_LOAD:{
        
        request->payload.copy.from=(uint8_t)v[6];      
        request->payload.copy.to=(uint8_t)v[7];       
        
        }

        case RPARAM_COPY:{
        
        request->payload.copy.from=(uint8_t)v[6];      
        request->payload.copy.to=(uint8_t)v[7];       
        
        }

       case RPARAM_REPLY:{

        request->payload.packed[0]=(uint8_t)v[6]; 

        }

} 

      gs_param_table_instance_t * tinst_tryout = gs_param_table_by_id(request->table_id);
      request->checksum = gs_param_table_checksum(tinst_tryout);

}
//end 



static void gs_rparam_process_packet(csp_conn_t * conn, csp_packet_t * request_packet)
{
    csp_packet_t * reply_packet = NULL;
    gs_rparam_query_t * reply;


    /* Handle endian */
    gs_rparam_query_t * request = (gs_rparam_query_t *) request_packet->data;


    request->length = csp_ntoh16(request->length);
    request->checksum = csp_ntoh16(request->checksum);
    
    //start of the  mutation probe
    FaultModel *fm_1 = _FAQAS_General_FM();
    mutate( request, fm_1 );
    _FAQAS_delete_FM(fm_1);
    //end of the mutation probe 

    /* Check valid mem */
    gs_param_table_instance_t * tinst = gs_param_table_by_id(request->table_id);
    if (tinst == NULL) {
        csp_buffer_free(request_packet);
        return;
    }


    switch(request->action) {
	case RPARAM_GET: {

    //start of the  mutation probe
    FaultModel *fm = _FAQAS_GET_FM();
    mutate( request, fm );
    _FAQAS_delete_FM(fm);
    //end of the mutation probe

            if (request->length > GS_RPARAM_QUERY_MAX_PAYLOAD) {
                log_error("Request too long %u", request->length);
                goto out_free;
            }

            /* Deal with endianness */
            for (unsigned int i = 0; i < request->length/2; i++) {
                request->payload.addr[i] = csp_ntoh16(request->payload.addr[i]);
            }

            if (!validate_checksum(tinst, request->checksum)) {

                
                log_error("Invalid table checksum: %u", request->checksum);
                goto out_free;
            }

            /* Save memory:
             * With full table requests we dont need the request packet no more */
            const bool full_table = (request->length == 0);
            if (full_table) {
                csp_buffer_free(request_packet);
                request_packet = NULL;
            }

            unsigned int param_pos = 0;
            const unsigned int param_max = full_table ? tinst->row_count : request->length/2;
            while (param_pos < param_max) {
                reply_packet = csp_buffer_get(RPARAM_QUERY_LENGTH(reply, GS_RPARAM_QUERY_MAX_PAYLOAD));
                if (reply_packet == NULL) {
                    goto out_free;
                }

                reply = (gs_rparam_query_t *) reply_packet->data;
                reply->action = RPARAM_REPLY;
                reply->table_id = gs_param_table_get_id(tinst);
                reply->checksum = gs_param_table_checksum_be(tinst);


               /* Get data */
                unsigned int length = 0;
                gs_error_t error;
                if (full_table) {
                    error = gs_param_serialize_full_table(tinst, &param_pos, F_TO_BIG_ENDIAN, reply->payload.packed, GS_RPARAM_QUERY_MAX_PAYLOAD, &length);
                } else {
                    error = gs_param_serialize_list(tinst, request->payload.addr, param_max, &param_pos, F_TO_BIG_ENDIAN, reply->payload.packed, GS_RPARAM_QUERY_MAX_PAYLOAD, &length);
                }

                if ((error == GS_ERROR_OVERFLOW) && (length > 0)) {
                    // No more room in package - this is ok
                    error = GS_OK;
                }

                /* If we did not serialize anything, abort */
                if (error || (length == 0)) {
                    log_error("Serialization failed, param_pos: %u, ser.length: %u, error: %d", param_pos, length, error);
                    csp_buffer_free(reply_packet);
                    goto out_free;
                }

                /* Set sequence */
                reply->seq = csp_hton16(param_pos);
                reply->total = csp_hton16(tinst->row_count);

                /* Set lengths */
                reply->length = csp_hton16(length);
                reply_packet->length = length + offsetof(gs_rparam_query_t, payload.packed);
               
                //start of the  mutation probe
                FaultModel *fm99 = _FAQAS_REPLY_FM();
                mutate( reply, fm99 );
                _FAQAS_delete_FM(fm99);
                //end of the mutation probe
                
                if (!csp_send(conn, reply_packet, 1000)) {
                    csp_buffer_free(reply_packet);
                    goto out_free;
                }
            }

            goto out_free;
	}
	case RPARAM_SET: {

    //start of the  mutation probe
    FaultModel *fm2 = _FAQAS_SET_FM();
    mutate( request, fm2 );
    _FAQAS_delete_FM(fm2);                                                                     
    //end of the mutation probe

            if (!validate_checksum(tinst, request->checksum)) {
                request_packet->data[0] = RPARAM_ERROR;
                goto out_send_status;
            }


            if (gs_param_deserialize(tinst, request->payload.packed, request->length, F_FROM_BIG_ENDIAN) == GS_OK) {
                request_packet->data[0] = RPARAM_SET_OK;
            } else {
                request_packet->data[0] = RPARAM_ERROR;
            }
            goto out_send_status;
	}
	case RPARAM_TABLE: {
#ifdef __AVR__
            csp_sfp_send_own_memcpy(conn, tinst->rows, tinst->row_count * sizeof(*(tinst->rows)), GS_RPARAM_QUERY_MAX_PAYLOAD, 10000, memcpy_P);
#else
            csp_sfp_send(conn, tinst->rows, tinst->row_count * sizeof(*(tinst->rows)), GS_RPARAM_QUERY_MAX_PAYLOAD, 10000);
#endif
            goto out_free;
	}
	case RPARAM_COPY: {


            // not supported in param 4.0
            log_warning("RPARAM_COPY - not supported: table-id: %u, from: %u, to: %u",
                        request->table_id, request->payload.copy.from, request->payload.copy.to);
            request_packet->data[0] = RPARAM_ERROR;
            goto out_send_status;
	}
	case RPARAM_LOAD: {

    //start of the  mutation probe
    FaultModel *fm4 = _FAQAS_LOAD_FM();
    mutate( request, fm4 );
    _FAQAS_delete_FM(fm4);                                                                     
    //end of the mutation probe

            log_debug("RPARAM_LOAD: table-id: %u, from: %u, to: %u",
                      request->table_id, request->payload.copy.from, request->payload.copy.to);
            if (gs_param_table_load(gs_param_table_by_id(request->payload.copy.to), NULL, NULL, 0) == GS_OK) {
                request_packet->data[0] = RPARAM_LOAD_OK;
            } else {
                request_packet->data[0] = RPARAM_ERROR;
            }
            goto out_send_status;
	}
	case RPARAM_SAVE: {

    //start of the  mutation probe
    FaultModel *fm5 = _FAQAS_SAVE_FM();
    mutate( request, fm5 );
    _FAQAS_delete_FM(fm5);                                                                     
    //end of the mutation probe

            log_debug("RPARAM_SAVE: table-id: %u, from: %u, to: %u",
                      request->table_id, request->payload.copy.from, request->payload.copy.to);
            if (gs_param_table_save(gs_param_table_by_id(request->payload.copy.from), NULL, NULL, 0) == GS_OK) {
                request_packet->data[0] = RPARAM_SAVE_OK;
            } else {
                request_packet->data[0] = RPARAM_ERROR;
            }
            goto out_send_status;
	}
	case RPARAM_LOAD_FROM_STORE: {
            gs_rparam_query_payload_store_t * store = (gs_rparam_query_payload_store_t *) &request->payload;
            log_debug("RPARAM_LOAD_FROM_STORE: table-id: %u, store: [%s], slot: [%s]",
                      request->table_id, store->store, store->slot);
            if (gs_param_table_load(gs_param_table_by_id(request->table_id), store->store, store->slot, 0) == GS_OK) {
                request_packet->data[0] = RPARAM_LOAD_OK;
            } else {
                request_packet->data[0] = RPARAM_ERROR;
            }
            goto out_send_status;
	}
	case RPARAM_SAVE_TO_STORE: {
            gs_rparam_query_payload_store_t * store = (gs_rparam_query_payload_store_t *) &request->payload;
            log_debug("RPARAM_SAVE_TO_STORE: table-id: %u, store: [%s], slot: [%s]",
                      request->table_id, store->store, store->slot);
            if (gs_param_table_save(gs_param_table_by_id(request->table_id), store->store, store->slot, 0) == GS_OK) {
                request_packet->data[0] = RPARAM_SAVE_OK;
            } else {
                request_packet->data[0] = RPARAM_ERROR;
            }
            goto out_send_status;
	}
	default: {
            request_packet->data[0] = RPARAM_ERROR;
            goto out_send_status;
        }
    }

out_send_status:
    request_packet->length = 1;
    if (csp_send(conn, request_packet, 0)) {
        return;
    }
out_free:
    if (request_packet != NULL) {
        csp_buffer_free(request_packet);
    }
}

gs_error_t gs_rparam_csp_service_handler(csp_conn_t * conn)
{
    if (conn) {
        csp_packet_t * packet;
        while ((packet = csp_read(conn, 0)) != NULL) {
            gs_rparam_process_packet(conn, packet);
        }

        csp_close(conn);
    }
    return GS_OK;
}
