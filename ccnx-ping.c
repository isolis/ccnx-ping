/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2014-2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright 2014-2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company. All Rights Reserved.
 */
#include <config.h>
#include <unistd.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <ccnx/api/ccnx_Portal/ccnx_Portal.h>
#include <ccnx/api/ccnx_Portal/ccnx_PortalRTA.h>

#include <parc/security/parc_Security.h>
#include <parc/security/parc_IdentityFile.h>
#include <parc/security/parc_PublicKeySignerPkcs12Store.h>

#include <parc/algol/parc_Memory.h>

PARCIdentity *
createAndGetIdentity(void)
{
    const char *keystoreName = "consumer_keystore";
    const char *keystorePassword = "keystore_password";
    unsigned int keyLength = 1024;
    unsigned int validityDays = 30;
    char *subjectName = "consumer";

    bool success = parcPublicKeySignerPkcs12Store_CreateFile(keystoreName, keystorePassword, subjectName, keyLength, validityDays);
    assertTrue(success,
               "parcPublicKeySignerPkcs12Store_CreateFile('%s', '%s', '%s', %d, %d) failed.",
               keystoreName, keystorePassword, subjectName, keyLength, validityDays);

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    parcIdentityFile_Release(&identityFile);

    return identity;
}

CCNxPortalFactory *
setupConsumerFactory(void)
{
    PARCIdentity *identity = createAndGetIdentity();

    CCNxPortalFactory *factory = ccnxPortalFactory_Create(identity);
    parcIdentity_Release(&identity);
    return factory;
}

int
consumer(void)
{
    parcSecurity_Init();
    
    CCNxPortalFactory *factory = setupConsumerFactory();
   
    CCNxPortal *portal = ccnxPortalFactory_CreatePortal(factory, ccnxPortalRTA_Message);

    assertNotNull(portal, "Expected a non-null CCNxPortal pointer.");

    CCNxName *name = ccnxName_CreateFromURI("lci:/Ping/World");

    CCNxInterest *interest = ccnxInterest_CreateSimple(name);
    ccnxName_Release(&name);

    CCNxMetaMessage *message = ccnxMetaMessage_CreateFromInterest(interest);

    struct timeval send_time;
    struct timeval receive_time;
	int ret;

	long latency       = 0;
	long total_latency = 0;
	long iterations    = 0;


	while(true){
	  gettimeofday(&send_time, NULL);
	  if (ccnxPortal_Send(portal, message,CCNxStackTimeout_Never)) {
		  while (ccnxPortal_IsError(portal) == false) {
			  CCNxMetaMessage *response = ccnxPortal_Receive(portal,CCNxStackTimeout_Never);
			  gettimeofday(&receive_time, NULL);
			  if (response != NULL) {
				  if (ccnxMetaMessage_IsContentObject(response)) {
					  CCNxContentObject *contentObject = ccnxMetaMessage_GetContentObject(response);

					  latency = 
						  (receive_time.tv_sec - send_time.tv_sec)*1000000 +
						  (receive_time.tv_usec - send_time.tv_usec);
					  total_latency = total_latency + latency;
			          iterations = iterations + 1;
					  printf("%6lu us  (%6lu us avg)\n", latency, total_latency/iterations);
						  
					  //PARCBuffer *payload = ccnxContentObject_GetPayload(contentObject);

					  //char *string = parcBuffer_ToString(payload);
					  //printf("%s\n", string);
					  //parcMemory_Deallocate((void **)&string);

					  break;
				  }
			  }
			  ccnxMetaMessage_Release(&response);
		  }
	  }
	  sleep(1);
    }

    ccnxPortal_Release(&portal);

    ccnxPortalFactory_Release(&factory);
    
    parcSecurity_Fini();
    return 0;
}

int
main(int argc, char *argv[argc])
{
    return consumer();
}
