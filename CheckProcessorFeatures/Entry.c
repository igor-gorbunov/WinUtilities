#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, TCHAR *argv[])
{
	if (0 != IsProcessorFeaturePresent(PF_FLOATING_POINT_PRECISION_ERRATA))
		_tprintf(_T("PF_FLOATING_POINT_PRECISION_ERRATA : present.\n"));
	else
		_tprintf(_T("PF_FLOATING_POINT_PRECISION_ERRATA : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_FLOATING_POINT_EMULATED))
		_tprintf(_T("PF_FLOATING_POINT_EMULATED : present.\n"));
	else
		_tprintf(_T("PF_FLOATING_POINT_EMULATED : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE_DOUBLE))
		_tprintf(_T("PF_COMPARE_EXCHANGE_DOUBLE : present.\n"));
	else
		_tprintf(_T("PF_COMPARE_EXCHANGE_DOUBLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_MMX_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_MMX_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_PPC_MOVEMEM_64BIT_OK))
		_tprintf(_T("PF_PPC_MOVEMEM_64BIT_OK : present.\n"));
	else
		_tprintf(_T("PF_PPC_MOVEMEM_64BIT_OK : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ALPHA_BYTE_INSTRUCTIONS))
		_tprintf(_T("PF_ALPHA_BYTE_INSTRUCTIONS : present.\n"));
	else
		_tprintf(_T("PF_ALPHA_BYTE_INSTRUCTIONS : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_XMMI_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_XMMI_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_3DNOW_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_3DNOW_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_3DNOW_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_RDTSC_INSTRUCTION_AVAILABLE))
		_tprintf(_T("PF_RDTSC_INSTRUCTION_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_RDTSC_INSTRUCTION_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_PAE_ENABLED))
		_tprintf(_T("PF_PAE_ENABLED : present.\n"));
	else
		_tprintf(_T("PF_PAE_ENABLED : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_XMMI64_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_XMMI64_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_SSE_DAZ_MODE_AVAILABLE))
		_tprintf(_T("PF_SSE_DAZ_MODE_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_SSE_DAZ_MODE_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_NX_ENABLED))
		_tprintf(_T("PF_NX_ENABLED : present.\n"));
	else
		_tprintf(_T("PF_NX_ENABLED : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_SSE3_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_SSE3_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_SSE3_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE128))
		_tprintf(_T("PF_COMPARE_EXCHANGE128 : present.\n"));
	else
		_tprintf(_T("PF_COMPARE_EXCHANGE128 : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_COMPARE64_EXCHANGE128))
		_tprintf(_T("PF_COMPARE64_EXCHANGE128 : present.\n"));
	else
		_tprintf(_T("PF_COMPARE64_EXCHANGE128 : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_CHANNELS_ENABLED))
		_tprintf(_T("PF_CHANNELS_ENABLED : present.\n"));
	else
		_tprintf(_T("PF_CHANNELS_ENABLED : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_XSAVE_ENABLED))
		_tprintf(_T("PF_XSAVE_ENABLED : present.\n"));
	else
		_tprintf(_T("PF_XSAVE_ENABLED : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ARM_VFP_32_REGISTERS_AVAILABLE))
		_tprintf(_T("PF_ARM_VFP_32_REGISTERS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_ARM_VFP_32_REGISTERS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ARM_NEON_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_ARM_NEON_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_ARM_NEON_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_SECOND_LEVEL_ADDRESS_TRANSLATION))
		_tprintf(_T("PF_SECOND_LEVEL_ADDRESS_TRANSLATION : present.\n"));
	else
		_tprintf(_T("PF_SECOND_LEVEL_ADDRESS_TRANSLATION : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_VIRT_FIRMWARE_ENABLED))
		_tprintf(_T("PF_VIRT_FIRMWARE_ENABLED : present.\n"));
	else
		_tprintf(_T("PF_VIRT_FIRMWARE_ENABLED : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_RDWRFSGSBASE_AVAILABLE))
		_tprintf(_T("PF_RDWRFSGSBASE_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_RDWRFSGSBASE_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_FASTFAIL_AVAILABLE))
		_tprintf(_T("PF_FASTFAIL_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_FASTFAIL_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE))
		_tprintf(_T("PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ARM_64BIT_LOADSTORE_ATOMIC))
		_tprintf(_T("PF_ARM_64BIT_LOADSTORE_ATOMIC : present.\n"));
	else
		_tprintf(_T("PF_ARM_64BIT_LOADSTORE_ATOMIC : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ARM_EXTERNAL_CACHE_AVAILABLE))
		_tprintf(_T("PF_ARM_EXTERNAL_CACHE_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_ARM_EXTERNAL_CACHE_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE))
		_tprintf(_T("PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE : not present.\n"));
	if (0 != IsProcessorFeaturePresent(PF_RDRAND_INSTRUCTION_AVAILABLE))
		_tprintf(_T("PF_RDRAND_INSTRUCTION_AVAILABLE : present.\n"));
	else
		_tprintf(_T("PF_RDRAND_INSTRUCTION_AVAILABLE : not present.\n"));

	return 1;
}
