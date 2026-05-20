# BCM Application Module
obj-$(USE_BCM_APP) += Bcm.o
obj-$(USE_BCM_APP) += Bcm_Lights.o
obj-$(USE_BCM_APP) += Bcm_SwInput.o
obj-$(USE_BCM_APP) += Bcm_CanApp.o

vpath-$(USE_BCM_APP) += $(ROOTDIR)/bcm_app
inc-$(USE_BCM_APP)   += $(ROOTDIR)/bcm_app
